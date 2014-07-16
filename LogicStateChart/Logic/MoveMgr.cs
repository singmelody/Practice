using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using RPGData;

namespace Logic
{
    public class MoveMgr : Singleton<MoveMgr>
    {

        public const int MIN_JOYSTICK_MOVE_RADIUS = 5;

        public void Init() { }

        public void KeepOnGround(Actor actor)
        {
            KeepOnGround(actor, false);
        }

        public void KeepOnGround(Actor actor, bool bNeedMoveCamera)
        {
            float height = 0.0f;
            if (!CommonUtility.IsOnGround(out height, actor.WorldPosition))
            {
                //移动摄像机
                if (bNeedMoveCamera)
                {
                    Vector3 distance = new Vector3(0, height - actor.WorldPosition.Y, 0);

					Debug.Printf("KeepOnGround is called " + distance.ToString() + "\n");
                    CameraMgr.Instance.Move(distance);
                }

                actor.WorldPosition = new Vector3(actor.WorldPosition.X, height, actor.WorldPosition.Z);
            }
        }

        public void RotateDependJoyStick(Actor actor)
        {
            //Vector2 dir = UserDefGUI.UserDefGUIRoot.Instance.JoystickDir;
            //if (dir != Vector2.Zero)
            //{
            //    Vector3 v3Dir = new Vector3(dir.X, 0.0f, dir.Y);
            //    Matrix44 camMat = CameraMgr.Instance.CameraTransfrom;
            //    Vector3 v3DirWorld = Matrix44.TransformDirection(camMat, v3Dir);
            //    v3DirWorld.Y = 0.0f;
            //    v3DirWorld.Normalize();
            //    Vector3 scale = actor.LocalScale;

            //    Matrix44 matActor = actor.LocalTransform;
            //    matActor.Backward = -v3DirWorld;
            //    matActor.Up = Vector3.Up;
            //    matActor.Right = Vector3.Cross(Vector3.Up, matActor.Backward);
            //    actor.LocalTransform = matActor;
            //    actor.LocalScale = scale;
            //}

        }

        public void Rotate(Actor actor, Vector3 targetBackwardVector3)
        {
            bool bClockwise = false;
            float fRadians = CommonUtility.CalcRotationRadians(actor.WorldTransform.Backward, targetBackwardVector3, out bClockwise);
            Matrix44 rotate = Matrix44.CreateRotationY(fRadians);
            rotate = Matrix44.Multiply(actor.WorldTransform, rotate);
            actor.WorldTransform = rotate;
        }

        public void Rotate(Actor actor, float fRadians)
        {
            Vector3 temp = actor.WorldTransform.Backward;
            Matrix44 rotate = Matrix44.CreateRotationY(fRadians);
            rotate = Matrix44.Multiply(actor.WorldTransform, rotate);
            actor.WorldTransform = rotate;
        }

        public void MovePostion(Actor actor, float fMoveSpeed)
        {
            MovePostion(actor, fMoveSpeed, false);
        }

        public void MovePostion(Actor actor, float fMoveSpeed, bool bNeedMoveCamera)
        {
            Vector3 moveDirectionNormalize = Vector3.Normalize(actor.WorldTransform.Backward);
            Vector3 moveDistance = moveDirectionNormalize * fMoveSpeed * Util.GetDeltaTime();

            //移动摄像机
            if (bNeedMoveCamera)
            {
                CameraMgr.Instance.Move(moveDistance);
            }

            actor.WorldPosition += moveDistance;
        }

        //TODO
        public void MovePostionOnGround(Actor actor, Vector3 dir, float fMoveSpeed, bool bNeedMoveCamera)
        {
            Vector3 moveDistance = dir * fMoveSpeed * Util.GetDeltaTime();
            Vector3 newPosition = actor.WorldPosition + moveDistance;

            Ray ray = new Ray();
            ray.Position = new Vector3(newPosition.X, 500.0f, newPosition.Z);
            ray.Direction = new Vector3(0, -1, 0);
            Actor test = IntersectWorld.IntersectWorld_Actor(ref ray, (LayerID)6);
            if (null == IntersectWorld.IntersectWorld_Actor(ref ray, (LayerID)6))
            {
                return;
            }

            //移动摄像机
            if (bNeedMoveCamera)
            {
				Debug.Printf("MovePostionOnGround is called " + moveDistance.ToString() + "\n");
                CameraMgr.Instance.Move(moveDistance);
            }

            actor.WorldPosition += moveDistance;
        }

        //TODO AutoMove
        //private void GenerateNavigationPath(AvatarData avatar, Vector3 endVector3)
        //{
        //    RecastFacade facade = RecastFacadeMgr.Instance.CurrentRecastFacade;
        //    facade.SetStartPos(avatar.AvatarActor.WorldPosition);
        //    facade.SetEndPos(endVector3);
        //    facade.FindOnePath();
        //    int iNum = facade.GetPosNodeCount();
        //    for (int ii = 0; ii < iNum; ++ii)
        //    {
        //        Vector3 node = new Vector3();
        //        facade.GetNextPosition(ref node);
        //        avatar.NavigationPathList.Add(node);
        //    }
        //}

        //TODO AutoMove
        //public void StartAutoMove(AvatarData avatar, Vector3 endVector3)
        //{
        //    GenerateNavigationPath(avatar, endVector3);
        //    avatar.IsAutoMove = true;
        //    avatar.AvatarState = CharacterState.Running;
        //}

        //TODO AutoMove
        //private void AutoMoveOver(AvatarData avatar)
        //{
        //    avatar.NavigationPathList.Clear();
        //    avatar.IsAutoMove = false;
        //    avatar.AvatarState = CharacterState.Idle;
        //}

        //TODO AutoMove
        //public void StopAutoMove(AvatarData avatar)
        //{
        //    AutoMoveOver(avatar);
        //}

    }
}