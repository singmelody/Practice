using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class Player : GameEntity
    {
        public Player()
        {
            Data = new PlayerData();
            Machine = new StateMachine(this);
            Machine.CurrentState = PlayerIdleState.Instance;
            OwnBBCollider = new PlayerBBCollider(this);
        }


        // Method
        public override void Update()
        {
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
            Machine.CurrentState = PlayerIdleState.Instance;
            OwnBBCollider.Init();
        }

        public void Reset()
        {
            Data.Reset();
            Machine.CurrentState = PlayerIdleState.Instance;
            OwnBBCollider.Init();
        }

        public void Clear()
        {
            Data.Clear();
        }
    }
}