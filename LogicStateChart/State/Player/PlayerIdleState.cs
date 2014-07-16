using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
	public class PlayerIdleState : Singleton<PlayerIdleState>, IState
    {
        public PlayerIdleState()
        {
        }

        // interface implement
        public void Enter(GameEntity entity)
        {
			Debug.Printf("Enter PlayerIdle State\n");
			ResetAnimation(entity);
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            CommonUtility.CrossFadingAnimation( entity, ConstDefine.PLAYER_IDLE_ANIMATION);
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }

        // util
        private void ResetAnimation(GameEntity entity)
        {
            entity.Data.AvatarActor.Animation.RemoveAffectedNodes(ConstDefine.PLAYER_RUN_ANIMATION, "Bip01", true);
            entity.Data.AvatarActor.Animation.RemoveAffectedNodes(ConstDefine.PLAYER_ATTACK_ANIMATION, "Bip01", true);
            entity.Data.AvatarActor.Animation.SetLayer(ConstDefine.PLAYER_ATTACK_ANIMATION, 0);
            entity.Data.AvatarActor.Animation.StopAnimation(ConstDefine.PLAYER_ATTACK_ANIMATION);
            entity.Data.AvatarActor.Animation.CrossFading(ConstDefine.PLAYER_IDLE_ANIMATION, 0.0f);
        }
    }
}