#define windows

using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using RPGData;
using UserDefGUI;
using MagicPads;
namespace Logic
{
    public class InputMgr : Singleton<InputMgr>
    {
        private const int MouseID = 100000;
        private Vector2 m_ScreenSize;

        public void Init()
        {
            m_ScreenSize.X = GraphicSystem.GetWidth();
            m_ScreenSize.Y = GraphicSystem.GetHeight();
        }

        public void Tick()
        {
            ProcessPlayerMove();
            ProcessPlayerAutoMove();
            ProcessPlayerCombat();
            ProcessCamera();
            ProcessEffect();
            MouseInput();
        }

        private void ProcessPlayerMove()
        {
            if (!SceneMgr.Instance.player.Data.IsAutoMove
                && !SceneMgr.Instance.player.Data.IsDied)
            {
                float fElapseTime = Util.GetDeltaTime();
                float fRotationRadians = (float)Math.PI * fElapseTime / 10;
                UpateMagicPad();
                if (UserDefGUIRoot.Instance.AttackButtonPressed)
                {
                    Message msg = new Message();

                    if (SceneMgr.Instance.player.Machine.CurrentState == PlayerRunState.Instance)
						msg.state = PlayerRunAttackState.Instance;
					else
                        msg.state = PlayerAttackState.Instance;

					MessageDispatcher.Instance.DispatchMessage( null, SceneMgr.Instance.player, msg);
					return;
                }
                if (UserDefGUIRoot.Instance.JoyStickIsCtrl())
                {
                    return;
                }
			
#if windows

                if( SceneMgr.Instance.player.Machine.CurrentState == PlayerIdleState.Instance
                    || SceneMgr.Instance.player.Machine.CurrentState == PlayerRunState.Instance
                    || SceneMgr.Instance.player.Machine.CurrentState == PlayerRunAttackState.Instance)
                {
                    if (Input.KeyPressed(Code.W))
                    {
						Debug.Printf("PlayerRunAttack State\n");
						Message msg = new Message();
						msg.state = PlayerRunAttackState.Instance;
						MessageDispatcher.Instance.DispatchMessage(null, SceneMgr.Instance.player, msg);
			
                        return;
                    }
					else if (Input.KeyPressed(Code.A))
                    {
                        Vector3 backwardVector3 = SceneMgr.Instance.player.Data.AvatarActor.WorldTransform.Backward;
                        float fRadians = CommonUtility.CalcRadians(backwardVector3);
                        if (fRotationRadians > fRadians)
                        {
                            fRadians += (float)Math.PI * 2;
                        }
                        fRadians -= fRotationRadians;
                        Vector3 toVector3 = new Vector3((float)Math.Sin(fRadians), 0, (float)Math.Cos(fRadians));
                        MoveMgr.Instance.Rotate(SceneMgr.Instance.player.Data.AvatarActor, toVector3);

                        Quaternion quat = SceneMgr.Instance.player.Data.AvatarActor.LocalRotation;
                        Vector3 dir = Quaternion.Rotate(quat, Vector3.UnitZ);
                        dir.Y = 0;
                        dir.Normalize();
                        SceneMgr.Instance.player.Data.AvatarDir = dir;
                    }
                    else if (Input.KeyPressed(Code.D))
                    {
                        Vector3 backwardVector3 = SceneMgr.Instance.player.Data.AvatarActor.WorldTransform.Backward;
                        float fRadians = CommonUtility.CalcRadians(backwardVector3);
                        fRadians += fRotationRadians;
                        if (Math.PI * 2 < fRadians)
                        {
                            fRadians -= (float)Math.PI * 2;
                        }
                        Vector3 toVector3 = new Vector3((float)Math.Sin(fRadians), 0, (float)Math.Cos(fRadians));
                        MoveMgr.Instance.Rotate(SceneMgr.Instance.player.Data.AvatarActor, toVector3);

                        Quaternion quat = SceneMgr.Instance.player.Data.AvatarActor.LocalRotation;
                        Vector3 dir = Quaternion.Rotate(quat, Vector3.UnitZ);
                        dir.Y = 0;
                        dir.Normalize();
                        SceneMgr.Instance.player.Data.AvatarDir = dir;
                    }
                    else
                    {
                        if (SceneMgr.Instance.player.CanEnterCharacterState(PlayerIdleState.Instance))
                        {
							Debug.Printf("Other Idle is Pressed\n");
                            Message msg = new Message();
                            msg.state = PlayerIdleState.Instance;
                            MessageDispatcher.Instance.DispatchMessage( null, SceneMgr.Instance.player, msg);
                        }
                    }
                }
#endif
				if (SceneMgr.Instance.player.CanEnterCharacterState(PlayerIdleState.Instance))
				{
					Message newMsg = new Message();
					newMsg.state = PlayerIdleState.Instance;
					MessageDispatcher.Instance.DispatchMessage(null, SceneMgr.Instance.player, newMsg);
				}

            }
        }
        private void UpateMagicPad()
        {
            TouchEvent[] tes = Input.GetCurrentTouchEvents();
            foreach (TouchEvent te in tes)
            {
                Vector2 pos = Input.GetScreenTouchPixelPosition(te.id);
                switch (te.eventType)
                {
                    case InputEventType.TouchMotionDown:
                        {
                            ScriptRoot.Root.MagicPad.OnTouchEvent(te.id,
                                MagicPad.TouchEvent.TouchMotionDown,
                                (int)pos.X, (int)pos.Y);
                            break;
                        }
                    case InputEventType.TouchMotionUp:
                        {
                            ScriptRoot.Root.MagicPad.OnTouchEvent(te.id,
                                MagicPad.TouchEvent.TouchMotionUp,
                                (int)pos.X, (int)pos.Y);
                            break;
                        }
                    case InputEventType.TouchMotionCancel:
                        {
                            ScriptRoot.Root.MagicPad.OnTouchEvent(te.id,
                                MagicPad.TouchEvent.TouchMotionCancel,
                                (int)pos.X, (int)pos.Y);
                            break;
                        }
                    case InputEventType.TouchMotionMove:
                        {
                            ScriptRoot.Root.MagicPad.OnTouchEvent(te.id,
                                MagicPad.TouchEvent.TouchMotionMove,
                                (int)pos.X, (int)pos.Y);

                            break;
                        }
                    default:
                        break;
                }
            }
            MouseEvent[] mes = Input.GetCurrentMouseEvents();
            if (mes.Length > 0)
            {
                Vector2 pos = Vector2.Zero;
                Input.GetScreenPosition(out pos);
                pos *= m_ScreenSize;
                foreach (MouseEvent me in mes)
                {
                    if ((me.eventType == InputEventType.MouseMove || me.button == MouseCode.LeftButton) && Input.MouseButtonPressed(MouseCode.LeftButton))
                    {
                        switch (me.eventType)
                        {
                            case InputEventType.MouseButtonDown:
                                {
                                    ScriptRoot.Root.MagicPad.OnTouchEvent(MouseID,
                                        MagicPad.TouchEvent.TouchMotionDown,
                                        (int)pos.X, (int)pos.Y);
                                    break;
                                }
							case InputEventType.MouseButtonUp:
                                {
                                    ScriptRoot.Root.MagicPad.OnTouchEvent(MouseID,
                                        MagicPad.TouchEvent.TouchMotionUp,
                                        (int)pos.X, (int)pos.Y);
                                    break;
                                }
                            case InputEventType.MouseMove:
                                {
                                    if (!Input.MouseButtonUp(MouseCode.LeftButton))
                                    {
                                        ScriptRoot.Root.MagicPad.OnTouchEvent(MouseID,
                                            MagicPad.TouchEvent.TouchMotionMove,
                                            (int)pos.X, (int)pos.Y);
                                    }

                                    break;
                                }
                            default:
                                break;
                        }
                    }
                }
            }

        }

        private void ProcessPlayerAutoMove()
        {
            if (!UserDefGUI.UserDefGUIRoot.Instance.JoyStickIsCtrl())
            {
                if ( PlayerIdleState.Instance == SceneMgr.Instance.player.Machine.CurrentState
                    || PlayerRunState.Instance == SceneMgr.Instance.player.Machine.CurrentState
                    )
                {
#if windows
                    if (Input.KeyDown(Code.Q))
                    {
                        if (!SceneMgr.Instance.player.Data.IsAutoMove)
                        {
                            NPC npc = NPCMgr.Instance.FindNPCByName("Npc_Chuansongshi_001");
                            //TODO AutoMove
                            //MoveMgr.Instance.StartAutoMove(Player.Instance.Data, npc.AvatarActor.WorldPosition);
                        }
                        else
                        {
                            //TODO AutoMove
                            //MoveMgr.Instance.StopAutoMove(Player.Instance.Data);
                        }
                    }
#endif
                }
            }
        }

        private void ProcessPlayerCombat()
        {
            if (PlayerIdleState.Instance == SceneMgr.Instance.player.Machine.CurrentState)
            {
#if windows
                if (Input.KeyDown(Code.J))
                {
                    if ( PlayerIdleState.Instance == SceneMgr.Instance.player.Machine.CurrentState 
                        || PlayerReadyCombatState.Instance == SceneMgr.Instance.player.Machine.CurrentState)
                    {
                        Message msg = new Message();
                        msg.state = PlayerAttackState.Instance;
                        MessageDispatcher.Instance.DispatchMessage(null, SceneMgr.Instance.player, msg);
                    }
                }
                else if (Input.KeyDown(Code.H))
                {
                    --SceneMgr.Instance.player.Data.AvatarHP;
                    Message msg = new Message();
                    msg.state = PlayerHurtState.Instance;
                    MessageDispatcher.Instance.DispatchMessage(null, SceneMgr.Instance.player, msg);
                }
#endif
            }
        }

        private void ProcessCamera()
        {
            float fElapseTime = Util.GetDeltaTime();
            float fRotationRadians = (float)Math.PI * fElapseTime / 10;
#if windows
            if (Input.KeyPressed(Code.N))
            {
                CameraMgr.Instance.RotateCamera(-fRotationRadians);
            }
            else if (Input.KeyPressed(Code.M))
            {
                CameraMgr.Instance.RotateCamera(fRotationRadians);
            }
            else if (Input.KeyPressed(Code.R))
            {
                CameraMgr.Instance.Reset();
            }
#endif
        }

        private void ProcessEffect()
        {
#if windows

#endif
        }

        //windows 
        public void MouseInput()
        {
#if windows
            if (Input.MouseButtonDown(MouseCode.LeftButton))
            {
                Vector2 screenPostion = new Vector2();
                Input.GetScreenPosition(out screenPostion);
                NPCChat chat = null;
                Ray ray = CommonUtility.ComputeMouseRay(screenPostion);
                Actor actor = IntersectWorld.IntersectWorld_Actor(ref ray, 0);
                if (null != actor)
                {
                    //string name = actor.Name;
                    if (null != actor.Parent)
                    {
                        actor = actor.Parent;
                    }
                    if (!actor.Name.StartsWith("NPC_"))
                    {
                        chat = null;
                    }

                    chat = PlayerChatMgr.Instance.PlayerChatToNPC(actor.Name);
                    PlayerChatPanelMgr.Instance.ResetPanel(chat, actor.Name);
                }
            }
#endif
        }

    }
}