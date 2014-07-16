using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;

namespace Logic
{
    public class Singleton<T> where T : class, new()
    {

        // Instead of compile time check, we provide a run time check
        // to make sure there is only one instance.
        protected Singleton()
        {

        }

        public static T Instance
        {
            get
            {
                return m_Instance;
            }

        }

        private static T m_Instance = new T();
    }
}