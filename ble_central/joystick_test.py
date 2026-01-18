import pygame
import sys
import os

os.environ["SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS"] = "1"
os.environ["SDL_LINUX_JOYSTICK_DEADZONES"] = "1"

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


try:
    pygame.init()
    pygame.joystick.init()
    pygame.font.init()

    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("ESP32 Wireless Gamepad Visualizer")
    clock = pygame.time.Clock()

    try:
        font = pygame.font.Font(None, 24)
    except Exception as e:
        font = pygame.font.SysFont("arial", 24)

    if pygame.joystick.get_count() == 0:
        print("No Joystick Found")
        sys.exit()
    
    joy = pygame.joystick.Joystick(0)
    joy.init()

    running = True

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        try:
            lx = joy.get_axis(0)
            ly = joy.get_axis(1)
            
            num_axes = joy.get_numaxes()
            rx = joy.get_axis(2) if num_axes > 2 else 0.0
            ry = joy.get_axis(3) if num_axes > 3 else 0.0

            pressed_buttons = []
            for i in range(joy.get_numbuttons()):
                if joy.get_button(i):
                    pressed_buttons.append(str(i))
            
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
    print(f"\n\nCrashed with Error: {e}")
    import traceback
    traceback.print_exc()

pygame.quit()