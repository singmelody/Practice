using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using RPGData;

namespace Logic
{
    public class CommonUtility
    {
        //计算与Z轴正方向的夹角，逆时针取值区间[0,2PI]
        public static float CalcRadians(Vector3 tempVector3)
        {
            float fRadians = (float)Math.Atan(tempVector3.X / tempVector3.Z);
            if (0 <= tempVector3.X)
            {
                if (0 < tempVector3.Z)
                {
                    return fRadians;
                }
                else
                {
                    if (0 >= fRadians)
                    {
                        return (float)Math.PI + fRadians;
                    }
                    else
                    {
                        return fRadians;
                    }
                }
            }
            else
            {
                if (0 > tempVector3.Z)
                {
                    return (float)Math.PI + fRadians;
                }
                else
                {
                    return (float)Math.PI * 2 + fRadians;
                }
            }
        }

        /// <summary>
        /// 计算两个朝向向量夹角的弧度值
        /// </summary>
        /// <param name="fromVector3">初始朝向</param>
        /// <param name="toVector3">目标朝向</param>
        /// <param name="bClockwise">是否顺时针旋转</param>
        /// <returns>旋转弧度值</returns>
        public static float CalcRotationRadians(Vector3 fromVector3, Vector3 toVector3, out bool bClockwise)
        {
            bClockwise = false;
            float fromRadians = CalcRadians(fromVector3);
            float toRadians = CalcRadians(toVector3);
            float fRotationRadians = toRadians - fromRadians;

            if (0 > fRotationRadians)
            {
                fRotationRadians += (float)Math.PI * 2;
            }

            if (Math.PI < fRotationRadians)
            {
                fRotationRadians -= (float)Math.PI * 2;
                bClockwise = true;
            }

            return fRotationRadians;
        }

        /// <summary>
        /// 根据UI坐标点生成朝向三维向量
        /// </summary>
        /// <param name="startPoint">起点</param>
        /// <param name="endPoint">终点</param>
        /// <returns>朝向三维向量</returns>
        public static Vector3 GenerateVector3ByUIIntPoint(ScriptGUI.IntPoint startPoint, ScriptGUI.IntPoint endPoint)
        {
            Vector3 tempVector3 = new Vector3(endPoint.left - startPoint.left, 0.0f, endPoint.top - startPoint.top);
            tempVector3.Normalize();
            return tempVector3;
        }

        public static bool IsOnGround(out float height, Vector3 pos)
        {
            Ray ray = new Ray();
            ray.Position = new Vector3(pos.X, 500.0f, pos.Z);
            ray.Direction = new Vector3(0, -1, 0);
            Vector3 groundPos;
            IntersectWorld.IntersectWorld_Point(ref ray, (LayerID)6, out groundPos);
            float heightlerp = pos.Y - groundPos.Y;
            height = groundPos.Y;
            if (Math.Abs(heightlerp) > 0.01f)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

		public static void MovePlayerOnGroud (GameEntity entity)
		{
            AvatarData data = entity.Data;
			Vector2 dir = UserDefGUI.UserDefGUIRoot.Instance.JoystickDir;
			if (dir != Vector2.Zero) {
				Vector3 v3Dir = new Vector3 (dir.X, 0.0f, dir.Y);
				Matrix44 camMat = CameraMgr.Instance.CameraTransfrom;

                data.m_PlayerDir = Matrix44.TransformDirection(camMat, v3Dir);
                data.m_PlayerDir.Y = 0.0f;
                data.m_PlayerDir.Normalize();
				Vector3 scale = entity.Data.AvatarActor.LocalScale;
				
				Matrix44 matActor = entity.Data.AvatarActor.LocalTransform;
				matActor.Backward = entity.Data.m_PlayerDir;
				matActor.Up = Vector3.Up;
				matActor.Right = Vector3.Cross (Vector3.Up, matActor.Backward);
				entity.Data.AvatarActor.LocalTransform = matActor;
				entity.Data.AvatarActor.LocalScale = scale;
			}

            MoveMgr.Instance.MovePostionOnGround(data.AvatarActor, entity.Data.m_PlayerDir, ConstDefine.PLAYER_RUN_SPEED, true);
			MoveMgr.Instance.KeepOnGround(data.AvatarActor, true);
		}

        public static void PlayAnimation(Actor actor, string sAnimationName)
        {
            if (null != actor)
            {
                actor.Animation.PlayAnimation(sAnimationName);
            }
        }

        public static void CrossFadingAnimation(GameEntity entity, string sAnimationName)
        {
            Actor actor = entity.Data.AvatarActor;
            if (null != actor)
            {
                actor.Animation.CrossFading(sAnimationName);
            }
        }


		public static void CrossFadingAnimation (GameEntity entity, string sAnimationName, float time)
		{
            Actor actor = entity.Data.AvatarActor;
			if( null != actor)
			{
				actor.Animation.CrossFading( sAnimationName, time);
			}
		}

		public static bool IsAnimationPlaying(GameEntity entity, string sAnimationName)
        {
            Actor actor = entity.Data.AvatarActor;
            if (null != actor)
            {
                return actor.Animation.IsAnimationPlaying(sAnimationName);
            }
            else
            {
                return false;
            }
        }

		public static bool IsArriveAnimationLastFrame(GameEntity entity, string sAnimationName)
        {
            Actor actor = entity.Data.AvatarActor;
            if (null != actor)
            {
                if (actor.Animation.IsAnimationPlaying(sAnimationName))
                {
                    float fTotalFrame = actor.Animation.GetCurrentAnimFrameCount();
                    float fCurrFrame = actor.Animation.GetCurrentFrame();
                    Debug.Printf("TotalFrame = " + fTotalFrame.ToString() + ",          CurrFrame = " + fCurrFrame.ToString() + "\n");
                    if (fTotalFrame <= fCurrFrame)
                    {
                        Debug.Printf("TotalFrame is samll than CurrFrame");
                        return true;
                    }
                    else
                    {
                        return Math.Abs(fTotalFrame - fCurrFrame) <= 1.0f;
                    }
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }

        /// <summary>
        /// 监测avatarData是否在播放sCurrentAnimation动画，
        /// 未播放，则开始播放（若当前处于CharacterState.Attack状态同时重置攻击计数），
        /// 播放中，监测是否播放到最后一帧，若播放到最后一帧则进入eNextState。
        /// 注意：动画必须为OnceStop播停类型
        /// </summary>
        /// <param name="avatarData">AvatarData实例对象</param>
        /// <param name="sCurrentAnimation">当前播放的动画</param>
        /// <param name="eNextState">即将进入的状态</param>
        /// <returns>是否刚刚结束播放sCurrentAnimation动画</returns>
        public static bool CheckAnimation(GameEntity entity, string sCurrentAnimation)
        {
            if (IsAnimationPlaying(entity, sCurrentAnimation))
            {
                if (IsArriveAnimationLastFrame(entity, sCurrentAnimation))
                    return true;
            }
            else
            {
                CrossFadingAnimation(entity, sCurrentAnimation);
                entity.Data.ResetDamageCountPerAttack();
            }

            return false;
        }

		public static bool CheckAnimation(GameEntity entity, string sCurrentAnimation, float time)
		{
			if (IsAnimationPlaying(entity, sCurrentAnimation))
			{
				if (IsArriveAnimationLastFrame(entity, sCurrentAnimation))
					return true;
			}
			else
			{
				CrossFadingAnimation(entity, sCurrentAnimation, time);
				entity.Data.ResetDamageCountPerAttack();
			}
			
			return false;
		}

        //从屏幕位置发射射线
        public static Ray ComputeMouseRay(Vector2 screenPos)
        {
            Vector3 screenCoord3D = new Vector3((screenPos.X - 0.5f) * 2.0f, (screenPos.Y - 0.5f) * 2.0f, 1.0f);
            Matrix44 viewM = CameraMgr.Instance.Camera.GetComponent<CameraComponent>().View;
            Matrix44 projM = CameraMgr.Instance.Camera.GetComponent<CameraComponent>().Proj;
            Matrix44 invProjM = Matrix44.Invert(projM);
            Matrix44 invViewM = Matrix44.Invert(viewM);

            Vector3 viewCoord = Matrix44.TransformPosition(invProjM, screenCoord3D);
            Vector3 localMousePos = viewCoord * CameraMgr.Instance.Camera.GetComponent<CameraComponent>().NearClipPlane * 1.0f;
            localMousePos.Y = -1 * localMousePos.Y;
            Vector3 worldMousePos = Matrix44.TransformPosition(invViewM, localMousePos);
            Vector3 worldMouseDir = worldMousePos - CameraMgr.Instance.Camera.WorldPosition;
            worldMouseDir = Vector3.Normalize(worldMouseDir);
            //Vector3 dir = new Vector3 (worldMouseDir.X,worldMouseDir.Y,worldMouseDir.Z);
            //dir = Vector3.Normalize(dir);
            return new Ray(worldMousePos, worldMouseDir);
        }

		// 始终朝向主角
		public static void FaceToPlayer(AvatarData data)
		{
			Vector3 distance = SceneMgr.Instance.player.Data.AvatarActor.WorldPosition - data.AvatarActor.WorldPosition;
			float length = distance.Length();
			
			if (Math.Abs(length) < ConstDefine.ENEMY_GUARD_RANGE )
			{
				Vector3 forward = -Vector3.Normalize(data.AvatarActor.WorldTransform.Forward);
				Vector3 nDistance = Vector3.Normalize(distance);
				
				float cosRotate = Vector3.Dot( forward, nDistance);
				float sinY = Vector3.Cross( forward, nDistance).Y;
				
				if (cosRotate < ConstDefine.ENEMY_VIEW_RANGE )
				{
					float angle = (float)(Math.Acos(cosRotate) * 180.0f / Math.PI);
					
					if(sinY<0)
						angle *= -1.0f;
					
					float magic = 0.02f / Math.Abs(cosRotate);
					Quaternion q = Quaternion.CreateFromAxisAngle(Vector3.UnitY, angle * Util.GetDeltaTime() * magic);
					data.AvatarActor.WorldRotation *= q;
				}		
			}
		}
    }
}