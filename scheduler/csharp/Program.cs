using System;
using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;

public class ScheduledTask<T>
{
    public Guid Id { get; } = Guid.NewGuid();
    public Func<Task<T>> TaskAction { get; }
    public Action<T?>? Callback { get; }
    public TimeSpan Interval { get; }
    public DateTime NextRun { get; set; }

    public ScheduledTask(Action action, TimeSpan interval)
    {
        Id = Guid.NewGuid();
        TaskAction = () => { action(); return default; };
        Interval = interval;
        NextRun = DateTime.UtcNow + interval;
    }

    public ScheduledTask(Func<Task<T>> action, TimeSpan interval)
    {
        Id = Guid.NewGuid();
        TaskAction = action;
        Interval = interval;
        NextRun = DateTime.UtcNow + interval;
    }

    public ScheduledTask(Func<Task<T>> action, TimeSpan interval, Action<T?>? callback)
    {
        Id = Guid.NewGuid();
        TaskAction = () => _ = action();
        Interval = interval;
        NextRun = DateTime.UtcNow + interval;
        Callback = callback;
    }
}

public class Scheduler<T>
{
    private readonly ConcurrentBag<ScheduledTask<T>> _tasks = new();
    private readonly CancellationTokenSource _cts = new();

    public Guid AddTask(Func<Task<T>> action, TimeSpan interval, Action<T?>? callback = null)
    {
        var task = new ScheduledTask<T>(action, interval, callback);
        _tasks.Add(task);
        return task.Id;
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
                        try
                        {
                            _ = Task.Run(() => task.TaskAction()); // fire-and-forget, suppress warning
                            if (task.Callback != null)
                            {
                                var result = task.TaskAction();
                                var x = await result;
                                Console.WriteLine($"I see the result as {result}");
                                task.Callback(x);
                            }
                        }
                        catch { /* handle exceptions as needed */ }
                        task.NextRun = now + task.Interval;
                    }
                }
                await Task.Delay(100, _cts.Token); // Check every 100ms
            }
        }, _cts.Token);
    }

    public void Stop() => _cts.Cancel();
}

public static class TaskFactory
{

}

// Example usage:
class Program
{
    static async Task Main()
    {
        var scheduler = new Scheduler<int>();

        var t1 = async () =>
        {
            Console.WriteLine("Task 1: " + DateTime.Now);
            await Task.Delay(1000);
            return new Random().Next(1, 100);
        };

        // scheduler.AddTask(() => Console.WriteLine("Task 1: " + DateTime.Now), TimeSpan.FromSeconds(1));
        // scheduler.AddTask(async () =>
        // {
        //     Console.WriteLine("Task 2: " + DateTime.Now);
        //     await Task.Delay(2000); // Simulate a longer-running task 
        //     Console.WriteLine("Task 2 completed: " + DateTime.Now);
        // }, TimeSpan.FromSeconds(5));

        scheduler.AddTask(t1, TimeSpan.FromSeconds(3), (result) => Console.WriteLine($"Callback says {result}"));

        scheduler.Start();

        Console.WriteLine("Press Enter to exit...");
        Console.ReadLine();
        scheduler.Stop();
    }
}