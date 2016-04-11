using System;
using System.Net.Sockets;
using System.Text;

namespace PiSockets
{
    class Program
    {
        static void Main(string[] args)
        {
            TcpClient tcp = new TcpClient();
            UTF8Encoding utf = new UTF8Encoding();
            byte[] mess = { 1 };
            string input = string.Empty;
            Console.WriteLine("What is the IP of the Raspberry pi?");
            string ip = Console.ReadLine();
            Console.WriteLine("What is the port of the Raspberry pi?");
            int port = int.Parse(Console.ReadLine());
            tcp.Connect(ip, port);

            Console.WriteLine("Connected to " + ip + ":" + port);
            while (mess[0]!=0)
            {
                input = Console.ReadLine();
                mess = utf.GetBytes(input);
                if(mess.Length > 0)
                {
                    mess[0] = (byte)(mess[0] - '0');
                    Console.WriteLine(mess[0]);
                    if (mess[0] != 0)
                    {
                        if (mess.Length <= 256)
                            tcp.GetStream().Write(mess, 0, mess.Length);
                        else
                            Console.WriteLine("Message is too long");
                    }
                    else
                    {
                        tcp.GetStream().Write(mess, 0, mess.Length);
                    }
                }
                else
                {
                    Array.Resize(ref mess, mess.Length + 1);
                    mess[0] = 5;
                }
            }
        }
    }
}
