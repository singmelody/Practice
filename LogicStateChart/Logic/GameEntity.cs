using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
	public class GameEntity
    {
        public AvatarData Data;
        public StateMachine Machine;
        public BBCollider OwnBBCollider;

        public virtual void Update() { }
        public virtual bool HandleMessage(Message msg) { return false; }

        // Need to remove later
        private bool CanEnterIdleState()
        {
			if (PlayerRunState.Instance == Machine.CurrentState && Data.IsAutoMove)
			{
				return false;
			}
			
			if (PlayerReadyCombatState.Instance == Machine.CurrentState
                || PlayerAttackState.Instance == Machine.CurrentState
                || PlayerDieState.Instance == Machine.CurrentState)
            {
                return false;
            }

            return true;
        }
        private bool CanEnterRunningState()
        {
            if (PlayerReadyCombatState.Instance == Machine.CurrentState
                || PlayerAttackState.Instance == Machine.CurrentState
                || PlayerCombatIdleState.Instance == Machine.CurrentState
                || PlayerHurtState.Instance == Machine.CurrentState
                || PlayerDieState.Instance == Machine.CurrentState)
            {
                return false;
            }

            return true;
        }
        private bool CanEnterReadyCombatState()
        {
            if (PlayerIdleState.Instance == Machine.CurrentState)
            {
                return true;
            }

            return false;
        }
        private bool CanEnterAttackState()
        {
            if (PlayerIdleState.Instance == Machine.CurrentState
                || PlayerReadyCombatState.Instance == Machine.CurrentState)
            {
                return true;
            }

            return false;
        }
        private bool CanEnterCombatIdleState()
        {
            if (PlayerAttackState.Instance == Machine.CurrentState)
            {
                return true;
            }

            return false;
        }
        private bool CanEnterHurtState()
        {
            if (PlayerDieState.Instance == Machine.CurrentState)
            {
                return false;
            }

            return true;
        }
        private bool CanEnterDieState()
        {
            return true;
        }
        public bool CanEnterCharacterState(IState state)
        {
            if (state == PlayerIdleState.Instance)
                return CanEnterIdleState();
            else if (state == PlayerRunState.Instance)
                return CanEnterRunningState();
            else if (state == PlayerReadyCombatState.Instance)
                return CanEnterReadyCombatState();
            else if (state == PlayerAttackState.Instance)
                return CanEnterAttackState();
            else if (state == PlayerCombatIdleState.Instance)
                return CanEnterCombatIdleState();
            else if (state == PlayerHurtState.Instance)
                return CanEnterHurtState();
            else if (state == PlayerDieState.Instance)
                return CanEnterDieState();

            return false;
        }
    }
}