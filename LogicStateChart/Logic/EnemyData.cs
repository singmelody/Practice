using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class EnemyData : AvatarData
    {
        public EnemyData(Actor actor) : base(actor)
        {
   
        }

        private void InitMoveParameters()
        {
            IsAutoMove = false;
            NavigationPathList.Clear();
        }

        public override void Init()
        {
            base.Init();

            InitMoveParameters();
            AvatarHP = ConstDefine.ENEMY_MAXHP;
        }

        public override void Reset()
        {
            base.Reset();

            InitMoveParameters();
            AvatarHP = ConstDefine.ENEMY_MAXHP;
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