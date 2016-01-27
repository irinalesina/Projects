﻿using System;
using System.Xml;
using System.Xml.XPath;
class MyApp
{
    static void Main()
    {
        XPathDocument doc = new XPathDocument(@"D:\irinalesina\C#\ClassWork\13_xPath\13_xPath\Cars.xml");
        XPathNavigator nav = doc.CreateNavigator();
        XPathNodeIterator iterator = nav.Select("//Car[Color/@metallic]");
        while (iterator.MoveNext())
        {
            XPathNodeIterator it = iterator.Current.Select("Manufactured");
            it.MoveNext();
            string manufactured = it.Current.Value;
            it = iterator.Current.Select("Model");
            it.MoveNext();
            string model = it.Current.Value;
            Console.WriteLine("{0} {1}", manufactured, model);
        }




        Console.ReadKey();
    }
}