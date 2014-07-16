using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
	public class PlayerRunAttackState : Singleton<PlayerRunAttackState>, IState
	{
		public PlayerRunAttackState()
		{
		}
		
		// interface implement
		public void Enter(GameEntity entity)
		{
			Debug.Printf("Enter PlayerRunAttack State\n");
            SeparateAnimation(entity);

			CommonUtility.CrossFadingAnimation(entity, ConstDefine.PLAYER_ATTACK_ANIMATION);
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

        // util
        private void SeparateAnimation(GameEntity player)
        {
            player.Data.AvatarActor.Animation.AddAffectedNodes(ConstDefine.PLAYER_RUN_ANIMATION, "Bip01", false);
            player.Data.AvatarActor.Animation.AddAffectedNodes(ConstDefine.PLAYER_RUN_ANIMATION, "Bip01 Pelvis", false);
            player.Data.AvatarActor.Animation.AddAffectedNodes(ConstDefine.PLAYER_RUN_ANIMATION, "Bip01 Spine", false);
            player.Data.AvatarActor.Animation.AddAffectedNodes(ConstDefine.PLAYER_RUN_ANIMATION, "Bip01 L Thigh", true);
            player.Data.AvatarActor.Animation.AddAffectedNodes(ConstDefine.PLAYER_RUN_ANIMATION, "Bip01 R Thigh", true);
            player.Data.AvatarActor.Animation.AddAffectedNodes(ConstDefine.PLAYER_ATTACK_ANIMATION, "Bip01 Spine1", true);
            player.Data.AvatarActor.Animation.SetLayer(ConstDefine.PLAYER_ATTACK_ANIMATION, 1);
        }
	}
}