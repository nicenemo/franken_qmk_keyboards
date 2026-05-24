# ##########################################################################
# QMK Configuration Options - rules.mk
# Keyboard: Boardsource Unicorne (RP2040 Split Keyboard)
# Customized for 'nicenemo' Keymap with Copilot Layer Configurations
# ##########################################################################

# ==========================================
# Core Hardware & Split Options
# ==========================================
SPLIT_KEYBOARD = yes          # Hard requirement for split layouts like the Unicorne

# ==========================================
# Input & Protocol Performance
# ==========================================
# NKRO_ENABLE = yes

# ==========================================
# Peripheral & Hardware Drivers
# ==========================================
POINTING_DEVICE_ENABLE = no   # Disables background checks for joysticks/mouse drift
ENCODER_ENABLE = no           # Change to 'yes' if you have physical rotary encoders
OLED_ENABLE = yes             # Enables OLED screen support

# ==========================================
# Lighting Frameworks
# ==========================================
RGB_MATRIX_ENABLE = yes       # Activates advanced underglow layer coloring

# ==========================================
# Core QMK Introspection Fix
# ==========================================
INTROSPECTION_FEATURES_ENABLE = no  # FIX: Stops automated checks for missing C structures

# ==========================================
# Code Compilation & Optimizations
# ==========================================
LTO_ENABLE = yes               # Link-Time Optimization compresses final binary size
