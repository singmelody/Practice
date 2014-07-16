using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
namespace Logic
{
    public class ShakeCamera
    {
		private Vector3 deltaPos = Vector3.Zero;
		private const float m_shakeTime = 0.1f;
		private float m_lastTime = 0.0f;
		// Use this for initialization

		void Start ()
		{
		}

		public void Update ()
		{
			m_lastTime += Util.GetDeltaTime ();

			if (m_lastTime < m_shakeTime) 
			{
				CameraMgr.Instance.Camera.LocalPosition -= deltaPos;
				deltaPos = ApiRandom.Instance.insideUnitSphere () / 3.0f;
				CameraMgr.Instance.Camera.LocalPosition += deltaPos;
			} 
			else
			{
				m_lastTime = 0.0f;
				CameraMgr.Instance.m_bShakeCamera = false;
			}
		}
	};
}