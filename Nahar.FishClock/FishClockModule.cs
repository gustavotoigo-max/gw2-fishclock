using System;
using System.ComponentModel.Composition;
using System.Threading.Tasks;
using Blish_HUD;
using Blish_HUD.Modules;
using Blish_HUD.Modules.Managers;
using Blish_HUD.Settings;
using Blish_HUD.Controls;
using Blish_HUD.Input;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace Nahar.FishClock
{
    public enum FishingMapSelection
    {
        Seitung_Province_Cantha,
        New_Kaineng_City_Cantha,
        The_Echovald_Wilds_Cantha,
        Dragon_End_Cantha,
        Gyala_Delve_Cantha,
        Kourna_Tyria,
        Core_Tyria,
        Orr_Tyria
    }

    [Export(typeof(Module))]
    public class FishClockModule : Module
    {
        private static readonly Logger Logger = Logger.GetLogger<FishClockModule>();

        private Label _fishingTimeLabel;
        private SettingEntry<FishingMapSelection> _selectedMapSetting;
        private SettingEntry<int> _labelPositionX;
        private SettingEntry<int> _labelPositionY;

        private bool _isDragging = false;
        private Point _dragOffset = Point.Zero;

        // Temporizador para otimizar o Update
        private double _runningTime = 0;

        [ImportingConstructor]
        public FishClockModule([Import("ModuleParameters")] ModuleParameters moduleParameters) : base(moduleParameters) { }

        protected override void DefineSettings(SettingCollection settings)
        {
            _selectedMapSetting = settings.DefineSetting(
                "SelectedFishingMapEnum",
                FishingMapSelection.Seitung_Province_Cantha,
                () => "Map to Monitor",
                () => "Choose which map you want to monitor the fishing time zone for."
            );

            _labelPositionX = settings.DefineSetting("LabelPosX", 300);
            _labelPositionY = settings.DefineSetting("LabelPosY", 200);
        }

        protected override void Initialize()
        {
        }

        protected override Task LoadAsync()
        {
            int savedX = _labelPositionX.Value;
            int savedY = _labelPositionY.Value;

            _fishingTimeLabel = new Label()
            {
                Text = "Loading Fishing Clock...",
                Size = new Point(450, 40),
                Location = new Point(savedX, savedY),
                Font = GameService.Content.DefaultFont18,
                TextColor = Color.White,
                ShowShadow = true,
                Parent = GameService.Graphics.SpriteScreen
            };

            _fishingTimeLabel.LeftMouseButtonPressed += OnLabelPressed;
            _fishingTimeLabel.LeftMouseButtonReleased += OnLabelReleased;

            return Task.CompletedTask;
        }

        private void OnLabelPressed(object sender, MouseEventArgs e)
        {
            if (GameService.Input.Keyboard.ActiveModifiers.HasFlag(ModifierKeys.Ctrl))
            {
                _isDragging = true;
                _dragOffset = GameService.Input.Mouse.Position - _fishingTimeLabel.Location;
            }
        }

        private void OnLabelReleased(object sender, MouseEventArgs e)
        {
            if (_isDragging)
            {
                _isDragging = false;
                _labelPositionX.Value = _fishingTimeLabel.Location.X;
                _labelPositionY.Value = _fishingTimeLabel.Location.Y;
            }
        }

        protected override void Update(GameTime gameTime)
        {
            if (_fishingTimeLabel == null) return;

            // Gerencia o arrastar do componente a cada frame
            if (_isDragging)
            {
                if (!GameService.Input.Keyboard.ActiveModifiers.HasFlag(ModifierKeys.Ctrl))
                {
                    _isDragging = false;
                    _labelPositionX.Value = _fishingTimeLabel.Location.X;
                    _labelPositionY.Value = _fishingTimeLabel.Location.Y;
                }
                else
                {
                    _fishingTimeLabel.Location = GameService.Input.Mouse.Position - _dragOffset;
                }
            }

            // OTIMIZAÇÃO: Executa a lógica matemática e atualização visual apenas 1 vez por segundo (1000ms)
            _runningTime += gameTime.ElapsedGameTime.TotalMilliseconds;
            if (_runningTime < 1000 && !_isDragging) return;
            _runningTime = 0;

            FishingMapSelection mapaSelecionado = _selectedMapSetting.Value;
            bool isCantha = false;
            string nomeAmigavel = "";

            switch (mapaSelecionado)
            {
                case FishingMapSelection.Seitung_Province_Cantha:
                    nomeAmigavel = "Seitung Province (Cantha)";
                    isCantha = true;
                    break;
                case FishingMapSelection.New_Kaineng_City_Cantha:
                    nomeAmigavel = "New Kaineng City (Cantha)";
                    isCantha = true;
                    break;
                case FishingMapSelection.The_Echovald_Wilds_Cantha:
                    nomeAmigavel = "The Echovald Wilds (Cantha)";
                    isCantha = true;
                    break;
                case FishingMapSelection.Dragon_End_Cantha:
                    nomeAmigavel = "Dragon's End (Cantha)";
                    isCantha = true;
                    break;
                case FishingMapSelection.Gyala_Delve_Cantha:
                    nomeAmigavel = "Gyala Delve (Cantha)";
                    isCantha = true;
                    break;
                case FishingMapSelection.Kourna_Tyria:
                    nomeAmigavel = "Domain of Kourna (Tyria)";
                    isCantha = false;
                    break;
                case FishingMapSelection.Core_Tyria:
                    nomeAmigavel = "Core Tyria";
                    isCantha = false;
                    break;
                case FishingMapSelection.Orr_Tyria:
                    nomeAmigavel = "Ruins of Orr (Tyria)";
                    isCantha = false;
                    break;
            }

            DateTime agoraUtc = DateTime.UtcNow;
            double minutosReaisPassadosJanela = (agoraUtc.Hour % 2) * 60 + agoraUtc.Minute + (agoraUtc.Second / 60.0);
            double totalInGameMinutes = minutosReaisPassadosJanela * 12.0;
            double currentInGameMinute = totalInGameMinutes % 1440;

            string estadoAtual = "Unknown";
            double minutosReaisRestantes = 0;
            Color corDoTexto = Color.White;

            if (isCantha)
            {
                if (currentInGameMinute >= 420 && currentInGameMinute < 480)
                {
                    estadoAtual = "DAWN 🌅";
                    minutosReaisRestantes = (480 - currentInGameMinute) / 12.0;
                    corDoTexto = Color.Orange;
                }
                else if (currentInGameMinute >= 480 && currentInGameMinute < 1140)
                {
                    estadoAtual = "DAY ☀️";
                    minutosReaisRestantes = (1140 - currentInGameMinute) / 12.0;
                    corDoTexto = Color.LightYellow;
                }
                else if (currentInGameMinute >= 1140 && currentInGameMinute < 1200)
                {
                    estadoAtual = "DUSK 🌇";
                    minutosReaisRestantes = (1200 - currentInGameMinute) / 12.0;
                    corDoTexto = Color.OrangeRed;
                }
                else
                {
                    estadoAtual = "NIGHT 🌙";
                    double minutosInGameAteAmanhecer = currentInGameMinute >= 1200 ? (1440 - currentInGameMinute) + 420 : 420 - currentInGameMinute;
                    minutosReaisRestantes = minutosInGameAteAmanhecer / 12.0;
                    corDoTexto = Color.SkyBlue;
                }
            }
            else
            {
                if (currentInGameMinute >= 300 && currentInGameMinute < 360)
                {
                    estadoAtual = "DAWN 🌅";
                    minutosReaisRestantes = (360 - currentInGameMinute) / 12.0;
                    corDoTexto = Color.Orange;
                }
                else if (currentInGameMinute >= 360 && currentInGameMinute < 1200)
                {
                    estadoAtual = "DAY ☀️";
                    minutosReaisRestantes = (1200 - currentInGameMinute) / 12.0;
                    corDoTexto = Color.LightYellow;
                }
                else if (currentInGameMinute >= 1200 && currentInGameMinute < 1260)
                {
                    estadoAtual = "DUSK 🌇";
                    minutosReaisRestantes = (1260 - currentInGameMinute) / 12.0;
                    corDoTexto = Color.OrangeRed;
                }
                else
                {
                    estadoAtual = "NIGHT 🌙";
                    double minutosInGameAteAmanhecer = currentInGameMinute >= 1260 ? (1440 - currentInGameMinute) + 300 : 300 - currentInGameMinute;
                    minutosReaisRestantes = minutosInGameAteAmanhecer / 12.0;
                    corDoTexto = Color.SkyBlue;
                }
            }

            _fishingTimeLabel.Text = $"[{nomeAmigavel}] {estadoAtual} - Left: {TimeSpan.FromMinutes(minutosReaisRestantes).ToString(@"mm\:ss")}";
            _fishingTimeLabel.TextColor = corDoTexto;
        }

        protected override void Unload()
        {
            if (_fishingTimeLabel != null)
            {
                _fishingTimeLabel.LeftMouseButtonPressed -= OnLabelPressed;
                _fishingTimeLabel.LeftMouseButtonReleased -= OnLabelReleased;

                _fishingTimeLabel.Dispose();
                _fishingTimeLabel = null;
            }
        }
    }
}