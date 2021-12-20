from position import Position
import pygame


def linear_curve(positions, t, screen, color, trigger=True):
    p0_x = (1 - t) * positions[0].x
    p0_y = (1 - t) * positions[0].y

    p1_x = t * positions[1].x
    p1_y = t * positions[1].y

    curve = (p0_x + p1_x, p0_y + p1_y)

    if trigger:
        pygame.draw.line(screen, (0, 0, 0), (positions[0].x, positions[0].y),
                         (positions[1].x, positions[1].y), 1)
        pygame.draw.line(screen, color, (positions[0].x, positions[0].y),
                         (int(curve[0]), int(curve[1])), 5)
        pygame.draw.circle(screen, color, (int(curve[0]), int(curve[1])), 8)
    elif not trigger:
        pygame.draw.circle(screen, color, (int(curve[0]), int(curve[1])), 8)
        return int(curve[0]), int(curve[1])


def bezier_curve(positions, t, screen, color, curve_list, green,
                 trigger=True):
    p0_x = pow((1 - t), 2) * positions[0].x
    p0_y = pow((1 - t), 2) * positions[0].y

    p1_x = 2 * (1 - t) * t * positions[1].x
    p1_y = 2 * (1 - t) * t * positions[1].y

    p2_x = t ** 2 * positions[2].x
    p2_y = t ** 2 * positions[2].y

    curve = (p0_x + p1_x + p2_x, p0_y + p1_y + p2_y)
    if trigger:
        pygame.draw.line(screen, (0, 0, 0), (positions[0].x, positions[0].y),
                         (positions[1].x, positions[1].y), 1)
        pygame.draw.line(screen, (0, 0, 0), (positions[1].x, positions[1].y),
                         (positions[2].x, positions[2].y), 1)

    curve_list.append((int(curve[0]), int(curve[1])))


width, height = 1600, 900
size = (width, height)

pygame.init()
screen = pygame.display.set_mode(size)

white = (235, 235, 235)
red = (242, 2, 2)
green = (2, 242, 102)


t = 0
speed = 0.0001
Quadratic_positions = [Position(400, 400, "P0"), Position(880, 450, "P1"),
                       Position(720, 200, "P2")]

quadratic_curve = []

run = True
while run:
    screen.fill(white)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                run = False

    bezier_curve(Quadratic_positions, t, screen, red, quadratic_curve, green)

    if len(quadratic_curve) > 2:
        pygame.draw.lines(screen, red, False, quadratic_curve, 5)

    if t >= 1:
        t = 0
        quadratic_curve.clear()

    t += speed
    pygame.display.update()
pygame.quit()
