import pygame
import sys
import os

# --- DEBUG SETTINGS ---
os.environ["SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS"] = "1"
os.environ["SDL_LINUX_JOYSTICK_DEADZONES"] = "1"

# --- CONFIGURATION ---
WIDTH, HEIGHT = 800, 400
BACKGROUND_COLOR = (30, 30, 30)
STICK_COLOR = (50, 50, 50)
DOT_COLOR = (0, 255, 100)
TEXT_COLOR = (200, 200, 200)
L_STICK_CENTER = (200, 200)
R_STICK_CENTER = (600, 200)
RADIUS = 80

def map_axis(val):
    if abs(val) < 0.1: return 0
    return val * RADIUS

print("--- STEP 1: Starting Python Script ---")

try:
    pygame.init()
    pygame.joystick.init()
    pygame.font.init()
    print("--- STEP 2: Init Modules Success ---")

    # 1. Create Window
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("ESP32 Gamepad Visualizer")
    clock = pygame.time.Clock()
    print("--- STEP 3: Window Created ---")

    # 2. Load Font
    # We wrap this in a try/except because fonts on Arch/Linux can be tricky
    try:
        font = pygame.font.Font(None, 24)
        print("--- STEP 4: Font Loaded Successfully ---")
    except Exception as e:
        print(f"!!! CRITICAL ERROR LOADING FONT: {e}")
        # Fallback to SysFont if default fails
        font = pygame.font.SysFont("arial", 24)

    # 3. Check Joystick
    if pygame.joystick.get_count() == 0:
        print("!!! NO JOYSTICK FOUND - CLOSING !!!")
        sys.exit()
    
    joy = pygame.joystick.Joystick(0)
    joy.init()
    print(f"--- STEP 5: Joystick '{joy.get_name()}' Initialized ---")
    print(f"    (Axes: {joy.get_numaxes()}, Buttons: {joy.get_numbuttons()})")

    running = True
    print("--- STEP 6: Entering Main Loop ---")
    
    while running:
        # Event Loop
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Read Data (Wrap in try/except to catch Axis errors)
        try:
            lx = joy.get_axis(0)
            ly = joy.get_axis(1)
            
            # Safe read for Right Stick
            num_axes = joy.get_numaxes()
            rx = joy.get_axis(2) if num_axes > 2 else 0.0
            ry = joy.get_axis(3) if num_axes > 3 else 0.0
            
        except Exception as e:
            print(f"Error reading axis: {e}")
            lx, ly, rx, ry = 0, 0, 0, 0

        # Draw
        screen.fill(BACKGROUND_COLOR)
        
        # Left Stick
        pygame.draw.circle(screen, STICK_COLOR, L_STICK_CENTER, RADIUS, 5)
        pygame.draw.circle(screen, DOT_COLOR, (L_STICK_CENTER[0] + map_axis(lx), L_STICK_CENTER[1] + map_axis(ly)), 15)
        
        # Right Stick
        pygame.draw.circle(screen, STICK_COLOR, R_STICK_CENTER, RADIUS, 5)
        pygame.draw.circle(screen, DOT_COLOR, (R_STICK_CENTER[0] + map_axis(rx), R_STICK_CENTER[1] + map_axis(ry)), 15)

        # Text
        try:
            msg = f"L:({lx:.1f},{ly:.1f}) R:({rx:.1f},{ry:.1f})"
            txt = font.render(msg, True, TEXT_COLOR)
            screen.blit(txt, (20, 20))
        except Exception as e:
            print(f"Font Render Error: {e}")

        pygame.display.flip()
        clock.tick(60)

except Exception as e:
    print(f"\n\n!!! CRASHED WITH ERROR: {e} !!!")
    import traceback
    traceback.print_exc()

pygame.quit()
print("--- Script Finished ---")