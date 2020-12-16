using System;
using System.Runtime.CompilerServices;

namespace Lofle
{
    public static class MonoClass
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static string cppFunction();

        public static void MonoMethod(string text)
        {
            Console.WriteLine($"Print From C#, (parameter: {text} / function call return: {cppFunction()})");
        }
    }
}