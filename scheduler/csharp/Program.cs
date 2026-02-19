using System;
using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;

public class ScheduledTask<T>
{
    public Guid Id { get; } = Guid.NewGuid();
    public Func<Task<T?>> TaskAction { get; }
    public Action<T?>? Callback { get; }
    public TimeSpan Interval { get; }
    public DateTime NextRun { get; set; }

    // For sync Action
    public ScheduledTask(Action action, TimeSpan interval)
    {
        TaskAction = () => { action(); return Task.FromResult(default(T)); };
        Interval = interval;
        NextRun = DateTime.UtcNow + interval;
    }

    // For sync Func<T>
    public ScheduledTask(Func<T?> action, TimeSpan interval)
    {
        TaskAction = () => Task.FromResult(action());
        Interval = interval;
        NextRun = DateTime.UtcNow + interval;
    }

    // For async Func<Task<T>>
    public ScheduledTask(Func<Task<T?>> action, TimeSpan interval, Action<T?>? callback = null)
    {
        TaskAction = action;
        Interval = interval;
        NextRun = DateTime.UtcNow + interval;
        Callback = callback;
    }
}

public class TaskRun
{
    public Guid TaskId { get; }
    public DateTime RunTime { get; }
    public Exception? Exception { get; set; }

    public TaskRun(Guid taskId, DateTime runTime, Exception? exception = null)
    {
        TaskId = taskId;
        RunTime = runTime;
        Exception = exception;
    }
}

public class Scheduler<T>
{
    private readonly ConcurrentBag<ScheduledTask<T?>> _tasks = new();
    private readonly CancellationTokenSource _cts = new();
    private readonly ConcurrentBag<TaskRun> _taskRuns = new();
    public event Action<TaskRun>? TaskRunAdded;

    public Guid AddTask(Func<T?> action, TimeSpan interval, Action<T?>? callback = null)
    {
        return AddTask(() => Task.FromResult(action()), interval, callback);
    }

    public Guid AddTask(Func<Task<T?>> action, TimeSpan interval, Action<T?>? callback = null)
    {
        var task = new ScheduledTask<T?>(action, interval, callback);
        _tasks.Add(task);
        return task.Id;
    }

    private void LogTaskRun(Guid id, DateTime now, Exception? ex = null)
    {
        var run = new TaskRun(id, now, ex);
        _taskRuns.Add(run);
        TaskRunAdded?.Invoke(run);
    }

    public void Start()
    {
        Task.Run(async () =>
        {
            while (!_cts.Token.IsCancellationRequested)
            {
                var now = DateTime.UtcNow;
                foreach (var task in _tasks)
                {
                    if (now >= task.NextRun)
                    {
                        _ = Task.Run(async () =>
                        {
                            var result = await task.TaskAction();
                            if (task.Callback != null)
                            {
                                task.Callback?.Invoke(result);
                            }
                            LogTaskRun(task.Id, now);
                        }).ContinueWith(t =>
                        {
                            if (t.Exception != null)
                            {
                                LogTaskRun(task.Id, now, t.Exception);
                            }
                        }, TaskContinuationOptions.OnlyOnFaulted);
                        task.NextRun = now + task.Interval;
                    }
                }
                await Task.Delay(100, _cts.Token); // Check every 100ms
            }
        }, _cts.Token);
    }

    public void Stop() => _cts.Cancel();
}
// Example usage:
class Program
{
    static async Task Main()
    {
        var scheduler = new Scheduler<int>();

        scheduler.TaskRunAdded += (run) =>
        {
            if (run.Exception != null)
            {
                Console.WriteLine($"Task {run.TaskId} failed at {run.RunTime}: {run.Exception}");
            }
        };

        var t1 = () =>
        {
            Console.WriteLine("Task 1: " + DateTime.Now);
            return new Random().Next(1, 100);
        };

        var t2 = async () =>
        {
            Console.WriteLine("Task 2: " + DateTime.Now);
            await Task.Delay(4000);
            return new Random().Next(1, 100);
        };

        scheduler.AddTask(t1, TimeSpan.FromSeconds(1));
        scheduler.AddTask(t2, TimeSpan.FromSeconds(5), (result) => Console.WriteLine($"Callback says {result}"));

        scheduler.Start();

        Console.WriteLine("Press Enter to exit...");
        Console.ReadLine();
        scheduler.Stop();
    }
}