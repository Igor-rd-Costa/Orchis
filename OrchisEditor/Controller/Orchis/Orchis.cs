using OrchisEditor.Controller.Editor;
using OrchisEditor.View.Editor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using Windows.ApplicationModel.Appointments;

namespace OrchisEditor.Controller.Orchis
{
    internal class Engine : OrchisInterface
    {
        private static bool m_IsHoveringSceneView = true;
        private static EditorMouseMoveEventCallback m_MouseMoveCallback = (e) =>
        {
            if (!m_IsHoveringSceneView)
                m_IsHoveringSceneView = true;
        };
        private Engine() { }

        public static bool IsHovering { get { return m_IsHoveringSceneView; } }

        public static void Init(IntPtr parentWindow, EditorCameraData data)
        {
            OrchisInit(parentWindow, data);
            Editor.RegisterMouseMoveEventCallback(m_MouseMoveCallback);
        }

        public static void Shutdown()
        {
            OrchisShutdown();
        }

        public static IntPtr GetMainWindowHandle()
        {
            return OrchisGetMainWindowHandle();
        }

        public static void RegisterWindowHover()
        {
            if (m_IsHoveringSceneView)
                m_IsHoveringSceneView = false;
        }

        public static void RegisterKeyDownEvent(Key key)
        {
            OrchisEventsRegisterKeyDownEvent(KeyToOrchisKey(key));
        }
        public static void RegisterKeyUpEvent(Key key)
        {
            OrchisEventsRegisterKeyUpEvent(KeyToOrchisKey(key));
        }

        internal class Window
        {
            private Window() { }

            public static void UpdateRect()
            {
                OrchisMainWindowUpdateRect();
            }
        }
        internal class Renderer
        {
            private Renderer() { }
        }

        internal class ComponentManager
        {
            internal class TransformComponent
            {
                public static void SetPosition(Guid componentId, Vector3 position)
                {
                    OrchisComponentManagerTransformComponentSetPosition(componentId, position);
                }
                public static void SetRotation(Guid componentId, Vector3 rotation)
                {
                    OrchisComponentManagerTransformComponentSetRotation(componentId, rotation);
                }
                public static void SetScale(Guid componentId, Vector3 scale)
                {
                    OrchisComponentManagerTransformComponentSetScale(componentId, scale);
                }
            }
        }

        internal class Editor
        {
            public static void SetCameraData(EditorCameraData data)
            {
                OrchisEditorSetEditorCameraData(data);
            }

            public static EditorCameraData GetCameraData()
            {
                return OrchisEditorGetEditorCameraData();
            }

            public static void RegisterMouseMoveEventCallback(EditorMouseMoveEventCallback callback)
            {
                OrchisEditorRegisterMouseMoveEventCallback(callback);
            }
            public static void RegisterCameraMoveEventCallback(EditorCameraMoveEventCallback callback)
            {
                OrchisEditorRegisterCameraMoveEventCallback(callback);
            }
        }

        public static OrchisKey KeyToOrchisKey(Key key)
        {
            switch (key)
            {
                case Key.None: return OrchisKey.KEY_UNKNOWN;
                case Key.Cancel: return OrchisKey.KEY_CANCEL;
                case Key.Back: return OrchisKey.KEY_BACK;
                case Key.Tab: return OrchisKey.KEY_TAB;
                case Key.Clear: return OrchisKey.KEY_CLEAR;
                case Key.Enter: return OrchisKey.KEY_UNKNOWN;
                case Key.Pause: return OrchisKey.KEY_PAUSE;
                case Key.Capital: return OrchisKey.KEY_CAPITAL;
                case Key.HangulMode: return OrchisKey.KEY_HANGUL;
                case Key.JunjaMode: return OrchisKey.KEY_JUNJA;
                case Key.FinalMode: return OrchisKey.KEY_FINAL;
                case Key.KanjiMode: return OrchisKey.KEY_KANJI;
                case Key.Escape: return OrchisKey.KEY_ESCAPE;
                case Key.ImeConvert: return OrchisKey.KEY_CONVERT;
                case Key.ImeNonConvert: return OrchisKey.KEY_NONCONVERT;
                case Key.ImeAccept: return OrchisKey.KEY_ACCEPT;
                case Key.ImeModeChange: return OrchisKey.KEY_MODECHANGE;
                case Key.Space: return OrchisKey.KEY_SPACE;
                case Key.PageUp: return OrchisKey.KEY_PRIOR;
                case Key.PageDown: return OrchisKey.KEY_NEXT;
                case Key.End: return OrchisKey.KEY_END;
                case Key.Home: return OrchisKey.KEY_HOME;
                case Key.Left: return OrchisKey.KEY_LEFT;
                case Key.Up: return OrchisKey.KEY_UP;
                case Key.Right: return OrchisKey.KEY_RIGHT;
                case Key.Down: return OrchisKey.KEY_DOWN;
                case Key.Select: return OrchisKey.KEY_SELECT;
                case Key.Print: return OrchisKey.KEY_PRINT;
                case Key.Execute: return OrchisKey.KEY_EXEC;
                case Key.PrintScreen: return OrchisKey.KEY_SNAPSHOT;
                case Key.Insert: return OrchisKey.KEY_INSERT;
                case Key.Delete: return OrchisKey.KEY_DELETE;
                case Key.Help: return OrchisKey.KEY_HELP;
                case Key.A: return OrchisKey.KEY_A;
                case Key.B: return OrchisKey.KEY_B;
                case Key.C: return OrchisKey.KEY_C;
                case Key.D: return OrchisKey.KEY_D;
                case Key.E: return OrchisKey.KEY_E;
                case Key.F: return OrchisKey.KEY_F;
                case Key.G: return OrchisKey.KEY_G;
                case Key.H: return OrchisKey.KEY_H;
                case Key.I: return OrchisKey.KEY_I;
                case Key.J: return OrchisKey.KEY_J;
                case Key.K: return OrchisKey.KEY_K;
                case Key.L: return OrchisKey.KEY_L;
                case Key.M: return OrchisKey.KEY_M;
                case Key.N: return OrchisKey.KEY_N;
                case Key.O: return OrchisKey.KEY_O;
                case Key.P: return OrchisKey.KEY_P;
                case Key.Q: return OrchisKey.KEY_Q;
                case Key.R: return OrchisKey.KEY_R;
                case Key.S: return OrchisKey.KEY_S;
                case Key.T: return OrchisKey.KEY_T;
                case Key.U: return OrchisKey.KEY_U;
                case Key.V: return OrchisKey.KEY_V;
                case Key.W: return OrchisKey.KEY_W;
                case Key.X: return OrchisKey.KEY_X;
                case Key.Y: return OrchisKey.KEY_Y;
                case Key.Z: return OrchisKey.KEY_Z;
                case Key.LWin: return OrchisKey.KEY_LWIN;
                case Key.RWin: return OrchisKey.KEY_RWIN;
                case Key.Apps: return OrchisKey.KEY_APPS;
                case Key.Sleep: return OrchisKey.KEY_SLEEP;
                case Key.NumPad0: return OrchisKey.KEY_NUMPAD0;
                case Key.NumPad1: return OrchisKey.KEY_NUMPAD1;
                case Key.NumPad2: return OrchisKey.KEY_NUMPAD2;
                case Key.NumPad3: return OrchisKey.KEY_NUMPAD3;
                case Key.NumPad4: return OrchisKey.KEY_NUMPAD4;
                case Key.NumPad5: return OrchisKey.KEY_NUMPAD5;
                case Key.NumPad6: return OrchisKey.KEY_NUMPAD6;
                case Key.NumPad7: return OrchisKey.KEY_NUMPAD7;
                case Key.NumPad8: return OrchisKey.KEY_NUMPAD8;
                case Key.NumPad9: return OrchisKey.KEY_NUMPAD9;
                case Key.Multiply: return OrchisKey.KEY_MULTIPLY;
                case Key.Add: return OrchisKey.KEY_ADD;
                case Key.Separator: return OrchisKey.KEY_SEPARATOR;
                case Key.Subtract: return OrchisKey.KEY_SUBTRACT;
                case Key.Decimal: return OrchisKey.KEY_DECIMAL;
                case Key.Divide: return OrchisKey.KEY_DIVIDE;
                case Key.F1: return OrchisKey.KEY_F1;
                case Key.F2: return OrchisKey.KEY_F2;
                case Key.F3: return OrchisKey.KEY_F3;
                case Key.F4: return OrchisKey.KEY_F4;
                case Key.F5: return OrchisKey.KEY_F5;
                case Key.F6: return OrchisKey.KEY_F6;
                case Key.F7: return OrchisKey.KEY_F7;
                case Key.F8: return OrchisKey.KEY_F8;
                case Key.F9: return OrchisKey.KEY_F9;
                case Key.F10: return OrchisKey.KEY_F10;
                case Key.F11: return OrchisKey.KEY_F11;
                case Key.F12: return OrchisKey.KEY_F12;
                case Key.F13: return OrchisKey.KEY_F13;
                case Key.F14: return OrchisKey.KEY_F14;
                case Key.F15: return OrchisKey.KEY_F15;
                case Key.F16: return OrchisKey.KEY_F16;
                case Key.F17: return OrchisKey.KEY_F17;
                case Key.F18: return OrchisKey.KEY_F18;
                case Key.F19: return OrchisKey.KEY_F19;
                case Key.F20: return OrchisKey.KEY_F20;
                case Key.F21: return OrchisKey.KEY_F21;
                case Key.F22: return OrchisKey.KEY_F22;
                case Key.F23: return OrchisKey.KEY_F23;
                case Key.F24: return OrchisKey.KEY_F24;
                case Key.NumLock: return OrchisKey.KEY_NUMLOCK;
                case Key.Scroll: return OrchisKey.KEY_SCROLL;
                case Key.LeftShift: return OrchisKey.KEY_LSHIFT;
                case Key.RightShift: return OrchisKey.KEY_RSHIFT;
                case Key.LeftCtrl: return OrchisKey.KEY_LCTRL;
                case Key.RightCtrl: return OrchisKey.KEY_RCTRL;
                case Key.LeftAlt: return OrchisKey.KEY_LALT;
                case Key.RightAlt: return OrchisKey.KEY_RALT;
                case Key.BrowserBack: return OrchisKey.KEY_BROWSER_BACK;
                case Key.BrowserForward: return OrchisKey.KEY_BROWSER_FORWARD;
                case Key.BrowserRefresh: return OrchisKey.KEY_BROWSER_REFRESH;
                case Key.BrowserStop: return OrchisKey.KEY_BROWSER_STOP;
                case Key.BrowserSearch: return OrchisKey.KEY_BROWSER_SEARCH;
                case Key.BrowserFavorites: return OrchisKey.KEY_BROWSER_FAVORITES;
                case Key.BrowserHome: return OrchisKey.KEY_BROWSER_HOME;
                case Key.VolumeMute: return OrchisKey.KEY_VOLUME_MUTE;
                case Key.VolumeDown: return OrchisKey.KEY_VOLUME_DOWN;
                case Key.VolumeUp: return OrchisKey.KEY_VOLUME_UP;
                case Key.MediaNextTrack: return OrchisKey.KEY_MEDIA_NEXT_TRACK;
                case Key.MediaPreviousTrack: return OrchisKey.KEY_MEDIA_PREV_TRACK;
                case Key.MediaStop: return OrchisKey.KEY_MEDIA_STOP;
                case Key.MediaPlayPause: return OrchisKey.KEY_MEDIA_PLAY_PAUSE;
                case Key.LaunchMail: return OrchisKey.KEY_LAUNCH_MAIL;
                case Key.SelectMedia: return OrchisKey.KEY_LAUNCH_MEDIA_SELECT;
                case Key.LaunchApplication1: return OrchisKey.KEY_LAUNCH_APP1;
                case Key.Oem1: return OrchisKey.KEY_OEM_1;
                case Key.OemPlus: return OrchisKey.KEY_OEM_PLUS;
                case Key.OemComma: return OrchisKey.KEY_OEM_COMMA;
                case Key.OemMinus: return OrchisKey.KEY_OEM_MINUS;
                case Key.OemPeriod: return OrchisKey.KEY_OEM_PERIOD;
                case Key.Oem2: return OrchisKey.KEY_OEM_2;
                case Key.Oem3: return OrchisKey.KEY_OEM_3;
                default: return OrchisKey.KEY_UNKNOWN;
            }
        }
    }
}
