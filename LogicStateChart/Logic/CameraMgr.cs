using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;

namespace Logic
{
    public enum CameraState
    {
        eNone,
        eRotateLeft,
        eRotateRight,
    }

    public class CameraMgr : Singleton<CameraMgr>
    {
        private Actor m_Camera;
        private CameraState m_State = CameraState.eNone;
        public bool    m_bShakeCamera = false;
        private ShakeCamera m_shakeCamera = new ShakeCamera();

        private Vector3 DefaultAdjustVector3()
        {
            float fAdjustX = 0.0f;
            float fAdjustY = ScriptGUI.GUI.GetScreenSize().width / 25;
            float fAdjustZ = ScriptGUI.GUI.GetScreenSize().height / 10;
            return new Vector3(fAdjustX, fAdjustY, fAdjustZ);
        }

        private float DefaultAdjustRadius()
        {
            Vector3 adjustVector3 = DefaultAdjustVector3();
            return (float)Math.Sqrt(adjustVector3.X * adjustVector3.X + adjustVector3.Z * adjustVector3.Z);
        }

        private void SetCameraWorldTransform()
        {
            Matrix44 viewMatrix44 = Matrix44.CreateLookAt(Camera.WorldPosition, SceneMgr.Instance.player.Data.AvatarActor.WorldPosition, new Vector3(0, 1, 0));
            viewMatrix44 = Matrix44.Invert(viewMatrix44);
            Camera.WorldTransform = viewMatrix44;
        }

        private void AdjustCamera()
        {
            Camera.WorldPosition = SceneMgr.Instance.player.Data.AvatarActor.WorldPosition;
            Camera.WorldPosition += DefaultAdjustVector3();
            SetCameraWorldTransform();
        }

        public CameraState State
        {
            get
            {
                return m_State;
            }
            set
            {
                m_State = value;
            }
        }

        public void Init()
        {
            if (null == Camera)
            {
                Camera = ActorManager.MainCameraActor;
                if (null == Camera)
                {
                    throw (new ArgumentException("CameraMgr.Init camera is null"));
                }

                AdjustCamera();
            }
        }

        public void Reset()
        {
            Camera = ActorManager.MainCameraActor;
            if (null == Camera)
            {
                throw (new ArgumentException("CameraMgr.Reset camera is null"));
            }

            AdjustCamera();
        }

        public void Tick()
        {
            if(m_bShakeCamera)
            {
                m_shakeCamera.Update();
            }

            if (CameraState.eNone != m_State)
            {
                float fElapseTime = Util.GetDeltaTime();
                float fRotationRadians = (float)Math.PI * fElapseTime / 10;

                if (CameraState.eRotateLeft == m_State)
                {
                    CameraMgr.Instance.RotateCamera(-fRotationRadians);
                }
                else if (CameraState.eRotateRight == m_State)
                {
                    CameraMgr.Instance.RotateCamera(fRotationRadians);
                }
            }
        }

        public Actor Camera
        {
            get
            {
                return m_Camera;
            }
            set
            {
                m_Camera = value;
            }
        }

        public void Move(Vector3 distance)
        {
            Camera.WorldPosition += distance;
        }

        public Matrix44 CameraTransfrom
        {
            get
            {
                return Camera.LocalTransform;
            }
        }

        public void RotateCamera(float fRotateRadians)
        {
            float fRadians = CommonUtility.CalcRadians(Camera.WorldPosition - SceneMgr.Instance.player.Data.AvatarActor.WorldPosition);
            fRadians += fRotateRadians;
            if (Math.PI * 2 < fRadians)
            {
                fRadians -= (float)Math.PI * 2;
            }
            float fDefaultAdjustRadius = DefaultAdjustRadius();
            Vector3 adjustVector3 = new Vector3((float)Math.Sin(fRadians) * fDefaultAdjustRadius, DefaultAdjustVector3().Y, (float)Math.Cos(fRadians) * fDefaultAdjustRadius);
            Camera.WorldPosition = SceneMgr.Instance.player.Data.AvatarActor.WorldPosition + adjustVector3;
            SetCameraWorldTransform();
        }

    }
}