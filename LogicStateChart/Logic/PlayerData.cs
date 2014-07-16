using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class PlayerData : AvatarData
    {
        public PlayerData()
        {

        }

        private void BindPlayerActor()
        {
            AvatarActor = ActorManager.FindActiveActor(ConstDefine.PLAYER_ACTORNAME);
            if (null == AvatarActor)
            {
                throw (new ArgumentException("PlayerData.BindPlayerActor PlayerActor is null"));
            }
        }

        public override void Init()
        {
            base.Init();

            BindPlayerActor();
            AvatarHP = ConstDefine.PLAYER_MAXHP;
        }

        public override void Reset()
        {
            base.Reset();

            BindPlayerActor();
            AvatarHP = ConstDefine.PLAYER_MAXHP;
        }

        public Vector3 FaceDirection
        {
            get
            {
                return AvatarActor.WorldTransform.Backward;
            }
        }
    }
}