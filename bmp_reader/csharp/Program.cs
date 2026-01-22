// See https://aka.ms/new-console-template for more information
using System.Buffers.Binary;

using var file = File.OpenRead("../shared_data/image1.bmp");
using var reader = new BinaryReader(file);

var buffer = new byte[54];
var bytesRead = file.Read(buffer, 0, buffer.Length);

int dataOffset = BinaryPrimitives.ReadInt32LittleEndian(buffer.AsSpan(10, 4));
int width = BinaryPrimitives.ReadInt32LittleEndian(buffer.AsSpan(18, 4));
int height = BinaryPrimitives.ReadInt32LittleEndian(buffer.AsSpan(22, 4));
ushort bitsPerPixel = BinaryPrimitives.ReadUInt16LittleEndian(buffer.AsSpan(28, 2));

Console.WriteLine($"{width} x {height}");
Console.WriteLine($"Bits per pixel {bitsPerPixel}");

file.Seek(dataOffset, SeekOrigin.Begin);

var paddedRowWidth = (width * 3 + 3) & (~3);

var colorCounts = new Dictionary<(byte, byte, byte), int>();
var rowBuffer = new byte[paddedRowWidth];
for(var y = 0; y < height; y++)
{
    var _ = file.Read(rowBuffer, 0, paddedRowWidth);
    for(var x = 0; x < width; x++)
    {
        byte blue = rowBuffer[x * 3];
        byte green = rowBuffer[x * 3 + 1];
        byte red = rowBuffer[x * 3 + 2];
        var key = (red, green, blue);
        colorCounts[key] = colorCounts.GetValueOrDefault(key) + 1;
    }
}

var topTen = colorCounts.ToList().OrderByDescending(kv => kv.Value).Take(10);

foreach(var item in topTen)
{
    Console.WriteLine($"R: {item.Key.Item1} G: {item.Key.Item2} B: {item.Key.Item3} - Count: {item.Value}");
}

file.Close();