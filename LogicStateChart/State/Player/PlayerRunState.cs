using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
	public class PlayerRunState : Singleton<PlayerRunState>, IState
    {
        public PlayerRunState()
        {
		
        }

        // interface implement
        public void Enter(GameEntity entity)
        {
			Debug.Printf("Enter PlayerRun State\n");
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute (GameEntity entity)
		{
            CommonUtility.CheckAnimation(entity, ConstDefine.PLAYER_RUN_ANIMATION, 0.1f);
            CommonUtility.MovePlayerOnGroud(entity);

        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }
}