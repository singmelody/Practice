using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class PlayerReadyCombatState : Singleton<PlayerReadyCombatState>, IState
    {
        public PlayerReadyCombatState()
        {
        }

        // interface implement
        public void Enter(GameEntity entity)
        {
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            if (!entity.CanEnterCharacterState(PlayerAttackState.Instance))
                return;

            bool bRet = CommonUtility.CheckAnimation( entity, ConstDefine.PLAYER_READYCOMBAT_ANIMATION);
            if(bRet)
                entity.Machine.ChangeState(PlayerAttackState.Instance);
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }

    public class PlayerAttackState : Singleton<PlayerAttackState>, IState
    {
        public PlayerAttackState()
        {
        }

        // interface implement
        public void Enter(GameEntity entity)
        {
            SoundMgr.Instance.PlaySound(SoundMgr.SOUND_PLAYERATTACK_NAMEHEAD, entity.Data.AvatarActor);
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            if(!entity.CanEnterCharacterState(PlayerCombatIdleState.Instance))
                return;

            bool bRet = CommonUtility.CheckAnimation(entity, ConstDefine.PLAYER_ATTACK_ANIMATION);
            if (bRet)
                entity.Machine.ChangeState(PlayerCombatIdleState.Instance);
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }





    public class PlayerCombatIdleState : Singleton<PlayerCombatIdleState>, IState
    {
        public PlayerCombatIdleState()
        {
        }

        // interface implement
        public void Enter(GameEntity entity)
        {
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            if (entity.CanEnterCharacterState(PlayerAttackState.Instance))
            {
                entity.Machine.NextState = PlayerAttackState.Instance;
                CommonUtility.CheckAnimation( entity, ConstDefine.PLAYER_COMBATIDLE_ANIMATION);
            }
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }

    public class PlayerDieState : Singleton<PlayerDieState>, IState
    {
        public PlayerDieState()
        {

        }

        // interface implement
        public void Enter(GameEntity entity)
        {
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            if (entity.Data.AvatarActor.IsActive)
            {
                UserDefGUI.UserDefGUIRoot.Instance.SetRestartMenuVis(true);
                entity.Data.AvatarActor.DeactiveWithChildren();
            }
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }
}