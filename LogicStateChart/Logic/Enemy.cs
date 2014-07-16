using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class Enemy : GameEntity
	{
        const float m_attackTime = 28.0f;

		public Enemy(Actor actor)
        {
            Data = new EnemyData(actor);
            Data.Init();

            Machine = new StateMachine(this);
            Machine.CurrentState = EnemyIdleState.Instance;

            OwnBBCollider = new EnemyBBCollider(this);
        }

        // Method
        public override void Update()
        {
            if (Machine.CurrentState != EnemyDieState.Instance)
                CommonUtility.FaceToPlayer(Data);

            Machine.Update();
        }

        public override bool HandleMessage(Message msg)
        {
            bool b = Machine.HandleMessage(msg);
            return b;
        }

        // Util
        public void Init()
        {
            Data.Init();
            OwnBBCollider.Init();

            Machine.CurrentState = EnemyIdleState.Instance;
        }

        public void Reset()
        {
            Data.Reset();
            Machine.CurrentState = EnemyIdleState.Instance;
            OwnBBCollider.Init();
        }

        public void Clear()
        {
            Data.Clear();
        }
	}
	
}
