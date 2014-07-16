using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;

namespace Logic
{
    public class ApiRandom : Singleton<ApiRandom>
    {

		public float GetRandom ()
		{
			return (float)m_rand.NextDouble();
		}

		public Vector3 insideUnitSphere()
		{
			Vector3 v3 = new Vector3();

			v3.X = GetRandom();
			v3.Y = GetRandom();
			v3.Z = GetRandom();

			return Vector3.Normalize(v3);
		}

        private static Random m_rand = new Random();

    }
}