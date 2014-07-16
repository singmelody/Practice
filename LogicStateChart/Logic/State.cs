using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    // -----------------------Common State Begin--------------------------------
    public class CommonState : Singleton<CommonState>, IState
    {
        public CommonState()
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

        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            entity.Machine.ChangeState(msg.state);

            return true;
        }
    }
    // -----------------------Common State End--------------------------------

    public class StateMachine
    {
        private GameEntity m_Owner;

        private IState  PreviousState;
		public  IState  CurrentState;
        public  IState  NextState;
        public  IState  GlobalState;

        public StateMachine(GameEntity owner)
        {
            m_Owner = owner;

            PreviousState = null;
            CurrentState = null;
            NextState = null;
            GlobalState = CommonState.Instance;
        }

        public void Update()
        {
            if (CurrentState != null)
                CurrentState.Execute(m_Owner);
        }

        public bool HandleMessage(Message msg) 
        {
            if (CurrentState != null && CurrentState.OnMessage(m_Owner, msg))
            {
                return true;
            }

            if (GlobalState != null && GlobalState.OnMessage(m_Owner, msg))
            {
                return true;
            }

            return false;
        }

        public void ChangeState(IState newState)
        {
            if (newState == null)
                return;

            PreviousState = CurrentState;

            CurrentState.Exit(m_Owner);

            CurrentState = newState;

            CurrentState.Enter(m_Owner);
        }
            
    }

    public class MessageDispatcher
    {
		public void Init ()
		{

		}

        public static MessageDispatcher Instance
        {
            get
            {
                if (null == m_Instance)
                {
                    m_Instance = new MessageDispatcher();
                }
                return m_Instance;
            }
        }

        private MessageDispatcher()
        {

        }

        private void Discharge(GameEntity receiver, Message msg)
        {
			Debug.Printf(msg.state.GetType().ToString() + "\n");
            if(!receiver.HandleMessage(msg))
            {
                Debug.Printf("Message not handled");
            }
        }


        public void DispatchMessage(GameEntity sender, GameEntity receiver, Message msg)
        {
            if (receiver == null)
                return;

            Discharge(receiver, msg);    
        }

        static MessageDispatcher m_Instance;

    }

    public interface IState
    {
        void Enter(GameEntity entity);

        void Execute(GameEntity entity);

        void Exit(GameEntity entity);

        bool OnMessage(GameEntity entity, Message msg);
    }

    public struct Message
    {
        public IState state;
    }

    public class EnemyIdleState : Singleton<EnemyIdleState>, IState
    {
        public EnemyIdleState()
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
            CommonUtility.CrossFadingAnimation(entity, ConstDefine.ENEMY_IDLE_ANIMATION);
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }
}