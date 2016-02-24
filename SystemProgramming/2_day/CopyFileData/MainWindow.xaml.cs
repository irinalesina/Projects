﻿using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace CopyFileData
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        static string src, dest;
        static object monitor = new object();

        public MainWindow()
        {
            InitializeComponent();
        }

        private void buttonSource_Click(object sender, RoutedEventArgs e)
        {
            textBoxSource.Text = this.GetFilePath();
        }

        private string GetFilePath()
        {
            OpenFileDialog checkFile = new OpenFileDialog();
            checkFile.InitialDirectory = @"C:\Users\Irishka\Desktop";

            if (checkFile.ShowDialog() == null)
                return "";

            return checkFile.FileName;
        }

        private void buttonDestination_Click(object sender, RoutedEventArgs e)
        {
            textBoxDestination.Text = this.GetFilePath();
        }

        private void buttonStart_Click(object sender, RoutedEventArgs e)
        {
            dest = textBoxDestination.Text;
            src = textBoxSource.Text;
            Thread threadCopy = new Thread(CopyFile);
            progressBarCopyProgress.IsIndeterminate = true;
            
            threadCopy.Start();

            lock (monitor)
            {
                progressBarCopyProgress.IsIndeterminate = false;
                
            }
            //MessageBox.Show("Copying has completed!");
        }

        static void CopyFile()
        {
            lock (monitor)
            {
                Thread.Sleep(10000);
                Stopwatch swTotal = Stopwatch.StartNew();
                Stopwatch swRead = new Stopwatch();
                Stopwatch swWrite = new Stopwatch();
                int numReads = 0;
                int numWrites = 0;
                using (var outputFile = File.Create(dest))
                {
                    using (var inputFile = File.OpenRead(src))
                    {
                        int CopyBufferSize = 1000;
                        var buffer = new byte[CopyBufferSize];
                        int bytesRead;
                        do
                        {
                            swRead.Start();
                            bytesRead = inputFile.Read(buffer, 0, CopyBufferSize);
                            swRead.Stop();
                            ++numReads;
                            if (bytesRead != 0)
                            {
                                swWrite.Start();
                                outputFile.Write(buffer, 0, bytesRead);
                                swWrite.Stop();
                                ++numWrites;
                            }
                        } while (bytesRead != 0);
                    }
                }
                swTotal.Stop();
                MessageBox.Show("Copying has completed!");
            }
        }
    }
}
