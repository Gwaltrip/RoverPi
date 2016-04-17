using System;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace PiRoverController
{
    [Activity(Label = "PiRoverController", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        static readonly object _syncLock = new object();

        TcpClient tcp;
        UTF8Encoding utf;
        Thread tcpWorker;
        bool connected = false;
        byte[] mess;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            tcp = new TcpClient();
            utf = new UTF8Encoding();
            mess = new byte[3];
            tcpWorker = new Thread(new ThreadStart(TcpRunner));
            mess[0] = 0x5;

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            // Get our button from the layout resource,
            // and attach an event to it
            Button forward = FindViewById<Button>(Resource.Id.ForwardButton);
            Button left = FindViewById<Button>(Resource.Id.LeftButton);
            Button right = FindViewById<Button>(Resource.Id.RightButton);
            Button backward = FindViewById<Button>(Resource.Id.BackwardButton);
            Button stop = FindViewById<Button>(Resource.Id.StopButton);
            Button connect = FindViewById<Button>(Resource.Id.Connect);
            connect.Click += delegate
            {
                if (!tcpWorker.IsAlive)
                {
                    tcpWorker.Start();
                }
            };
            forward.Click += delegate
            {
                mess[0] = 0x1;
            };
            backward.Click += delegate
            {
                mess[0] = 0x2;
            };
            left.Click += delegate
            {
                mess[0] = 0x4;
            };
            right.Click += delegate
            {
                mess[0] = 0x3;
            };
            stop.Click += delegate
            {
                mess[0] = 0x5;
            };

        }

        protected void TcpRunner()
        {

            if (!connected)
            {
                try
                {
                    tcp.Connect("192.168.0.200", 8080);
                    connected = true;
                    Thread.Sleep(5000);
                    while (true)
                    {
                        tcp.GetStream().Write(mess, 0, 1);
                        Thread.Sleep(100);
                    }
                }
                catch (Exception e)
                {
                    AlertDialog.Builder alert = new AlertDialog.Builder(this);

                    alert.SetTitle(e.StackTrace);

                    alert.SetPositiveButton("Ok", (senderAlert, args) => {
                    });
                    RunOnUiThread(() =>
                    {
                        alert.Show();
                    });
                }
                finally
                {
                    connected = false;
                }
            }
        }
    }
}

