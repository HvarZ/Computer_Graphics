import math
from PIL import Image
from PIL import ImageDraw


def get_intensity(pixels, x, y, current_pixel):
    r = math.fabs(pixels[x, y][0] - current_pixel[0])
    g = math.fabs(pixels[x, y][1] - current_pixel[1])
    b = math.fabs(pixels[x, y][2] - current_pixel[2])

    return math.sqrt(r ** 2 + g ** 2 + b ** 2)


def get_sobel_pixel(pixel, x, y):
    if x == 0 or y == 0:
        return 0
    current_pixel = pixel[x, y]
    z1 = get_intensity(pixel, x - 1, y - 1, current_pixel)
    z2 = get_intensity(pixel, x, y - 1, current_pixel)
    z3 = get_intensity(pixel, x + 1, y - 1, current_pixel)
    z4 = get_intensity(pixel, x - 1, y, current_pixel)
    z6 = get_intensity(pixel, x + 1, y, current_pixel)
    z7 = get_intensity(pixel, x - 1, y + 1, current_pixel)
    z8 = get_intensity(pixel, x, y + 1, current_pixel)
    z9 = get_intensity(pixel, x + 1, y + 1, current_pixel)
    g_x = z7 + 2 * z8 + z9 - (z1 + 2 * z2 + z3)
    g_y = z3 + 2 * z6 + z9 - (z1 + 2 * z4 + z7)

    return math.sqrt(g_x ** 2 + g_y ** 2)


def get_gradient_image(img):
    image = Image.open(img)
    image_buf = Image.open(img)
    pixels = image.load()
    draw = ImageDraw.Draw(image_buf)
    width = image.size[0]
    height = image.size[1]
    for x in range(0, width):
        for y in range(0, height):
            if x == width - 1 or y == height - 1:
                draw.point((x, y), (0, 0, 0))
                continue
            gradient = get_sobel_pixel(pixels, x, y)
            if gradient < 5:
                gradient = 0
            color = int(gradient * 0.8)
            draw.point((x, y), (color, color, color))

    image_buf.save('images/result.png')


get_gradient_image('images/img.png')
