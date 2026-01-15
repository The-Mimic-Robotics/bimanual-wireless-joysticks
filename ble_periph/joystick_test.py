import pygame
import sys

import os
# This forces SDL to accept generic controllers
os.environ["SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS"] = "1"
os.environ["SDL_LINUX_JOYSTICK_DEADZONES"] = "1"

# --- CONFIGURATION ---
# Screen settings
WIDTH, HEIGHT = 800, 400
BACKGROUND_COLOR = (30, 30, 30)
STICK_COLOR = (50, 50, 50)
DOT_COLOR = (0, 255, 100)  # Green
CLICK_COLOR = (255, 50, 50) # Red (when clicked)
TEXT_COLOR = (200, 200, 200)

# Stick layout
L_STICK_CENTER = (200, 200)
R_STICK_CENTER = (600, 200)
RADIUS = 80

def map_axis(val):
    """Maps raw axis (-1.0 to 1.0) to pixel offset."""
    # Deadzone to stop jitter
    if abs(val) < 0.1:
        return 0
    return val * RADIUS

def main():
    pygame.init()
    pygame.joystick.init()

    # Create Window
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("ESP32 Gamepad Visualizer")
    clock = pygame.time.Clock()

    # --- THE FIX IS HERE ---
    pygame.font.init() 
    # Use 'None' to use the default internal pygame font, size 24
    font = pygame.font.Font(None, 24) 
    # -----------------------

    # Check connection
    if pygame.joystick.get_count() == 0:
        print("No joystick found! Check bluetooth connection.")
        # Optional: Clean exit so it doesn't just hang
        pygame.quit()
        sys.exit()

    # Initialize Controller
    joy = pygame.joystick.Joystick(0)
    joy.init()
    print(f"Connected to: {joy.get_name()}")
    print(f"Axes: {joy.get_numaxes()}, Buttons: {joy.get_numbuttons()}")

    running = True
    while running:
        # 1. Handle Events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # 2. Get Data
        # Left Stick (Usually Axis 0 and 1)
        lx = joy.get_axis(0)
        ly = joy.get_axis(1)
        
        # Right Stick (Usually Axis 2 and 3, sometimes 3 and 4 depending on OS)
        # Note: On Linux, axes mapping can be different than Windows. 
        # If the right stick doesn't move, try changing 2 and 3 to 3 and 4 below.
        rx = joy.get_axis(2)
        ry = joy.get_axis(3)

        # Get all pressed buttons to find the "clicks"
        pressed_buttons = []
        for i in range(joy.get_numbuttons()):
            if joy.get_button(i):
                pressed_buttons.append(str(i))

        # 3. Draw Screen
        screen.fill(BACKGROUND_COLOR)

        # --- DRAW LEFT STICK ---
        pygame.draw.circle(screen, STICK_COLOR, L_STICK_CENTER, RADIUS, 5)
        # Calculate dot position
        l_dot_pos = (L_STICK_CENTER[0] + map_axis(lx), L_STICK_CENTER[1] + map_axis(ly))
        pygame.draw.circle(screen, DOT_COLOR, l_dot_pos, 15)
        
        # Label
        text_l = font.render(f"L-Stick ({lx:.2f}, {ly:.2f})", True, TEXT_COLOR)
        screen.blit(text_l, (L_STICK_CENTER[0] - 80, L_STICK_CENTER[1] + 100))

        # --- DRAW RIGHT STICK ---
        pygame.draw.circle(screen, STICK_COLOR, R_STICK_CENTER, RADIUS, 5)
        # Calculate dot position
        r_dot_pos = (R_STICK_CENTER[0] + map_axis(rx), R_STICK_CENTER[1] + map_axis(ry))
        pygame.draw.circle(screen, DOT_COLOR, r_dot_pos, 15)
        
        # Label
        text_r = font.render(f"R-Stick ({rx:.2f}, {ry:.2f})", True, TEXT_COLOR)
        screen.blit(text_r, (R_STICK_CENTER[0] - 80, R_STICK_CENTER[1] + 100))

        # --- DRAW BUTTONS ---
        # If your "click" buttons are