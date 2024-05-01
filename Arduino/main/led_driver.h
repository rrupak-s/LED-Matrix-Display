#ifndef _LED_DRIVER_H__
#define __LED_DRIVER_H__
#include <inttypes.h>

class led_driver
{
private:
    uint8_t latch_pin_x, clock_pin_x, data_pin_x,
        latch_pin_y, clock_pin_y, data_pin_y;
    uint32_t X0 = 0, X1 = 8388608, Y0 = 16777215, Y1 = 8388607;
    /*  Relation between consecutive coordinates:
     *  Xn = X1/2^(n-1)
     *  Yn = Y(n-1) + Xn
     */
    uint32_t X_cord[22] =
        {X0, X1, X1 / 2, X1 / 4, X1 / 8, X1 / 16, X1 / 32,
         X1 / 64, X1 / 128, X1 / 256, X1 / 512, X1 / 1024,
         X1 / 2048, X1 / 4096, X1 / 8192, X1 / 16384, X1 / 32768,
         X1 / 65536, X1 / 131072, X1 / 262144, X1 / 524288, X1 / 1048576};

    uint32_t Y_cord[22] =
        {Y0, Y1, (Y1 + X_cord[2]), (Y_cord[2] + X_cord[3]),
         (Y_cord[3] + X_cord[4]), (Y_cord[4] + X_cord[5]), (Y_cord[5] + X_cord[6]),
         (Y_cord[6] + X_cord[7]), (Y_cord[7] + X_cord[8]), (Y_cord[8] + X_cord[9]),
         (Y_cord[9] + X_cord[10]), (Y_cord[10] + X_cord[11]), (Y_cord[11] + X_cord[12]),
         (Y_cord[12] + X_cord[13]), (Y_cord[13] + X_cord[14]), (Y_cord[14] + X_cord[15]),
         (Y_cord[15] + X_cord[16]), (Y_cord[16] + X_cord[17]), (Y_cord[17] + X_cord[18]),
         (Y_cord[18] + X_cord[19]), (Y_cord[19] + X_cord[20]), (Y_cord[20] + X_cord[21])};

    void ShiftVCC(uint32_t x)
    {

        uint8_t h_data = (x >> 16) & (0xff); // 1-8
        uint8_t m_data = (x >> 8) & (0xff);  // 9-16
        uint8_t l_data = (x >> 0) & (0xff);  // 17-24

        digitalWrite(latch_pin_x, LOW);
        shiftOut(data_pin_x, clock_pin_x, LSBFIRST, l_data);
        shiftOut(data_pin_x, clock_pin_x, LSBFIRST, m_data);
        shiftOut(data_pin_x, clock_pin_x, LSBFIRST, h_data);
        digitalWrite(latch_pin_x, HIGH);
    }
    void ShiftGND(uint32_t y)
    {
        uint8_t h_data = (y >> 16) & (0xff); // 1-8
        uint8_t m_data = (y >> 8) & (0xff);  // 9-16
        uint8_t l_data = (y >> 0) & (0xff);  // 17-24

        digitalWrite(latch_pin_y, LOW);
        shiftOut(data_pin_y, clock_pin_y, LSBFIRST, l_data);
        shiftOut(data_pin_y, clock_pin_y, LSBFIRST, m_data);
        shiftOut(data_pin_y, clock_pin_y, LSBFIRST, h_data);
        digitalWrite(latch_pin_y, HIGH);
    }

public:
    led_driver(uint8_t latch_pin_x_, uint8_t clock_pin_x_, uint8_t data_pin_x_,
               uint8_t latch_pin_y_, uint8_t clock_pin_y_, uint8_t data_pin_y_)
    {
        latch_pin_x = latch_pin_x_;
        clock_pin_x = clock_pin_x_;
        data_pin_x = data_pin_x_;
        latch_pin_y = latch_pin_y_;
        clock_pin_y = clock_pin_y_;
        data_pin_y = data_pin_y_;
        pinMode(latch_pin_x, OUTPUT);
        pinMode(data_pin_x, OUTPUT);
        pinMode(clock_pin_x, OUTPUT);
        pinMode(latch_pin_y, OUTPUT);
        pinMode(data_pin_y, OUTPUT);
        pinMode(clock_pin_y, OUTPUT);
    }

    void reset()
    { // to turn off all leds
        ShiftVCC(X0);
        ShiftGND(Y0);
    }

    void make_coordinate(int x, int y)
    {
        ShiftVCC(X_cord[x]);
        ShiftGND(Y_cord[y]);
        reset();
    }
    void make_line(int x1, int y1, int x2, int y2)
    {
        int steps, x, y, i = 0;
        float dy = (float)y2 - y1;
        float dx = (float)x2 - x1;

        if (dx > dy)
        {
            steps = dx;
        }
        else
        {
            steps = dy;
        }
        dx = dx / steps;
        dy = dy / steps;

        x = x1;
        y = y1;

        while (i <= steps)
        {
            make_coordinate(x, y);

            x = x + dx;
            y = y + dy;
            i++;
        }
    }

    void make_circle(int xc, int yc, int r)
    {
        int x, y, p;
        x = 0;
        y = r;
        while (x < y)
        {
            make_coordinate(x + xc, y + yc);
            make_coordinate(y + xc, x + yc);
            make_coordinate(x + xc, -y + yc);
            make_coordinate(-y + xc, x + yc);
            make_coordinate(-x + xc, -y + yc);
            make_coordinate(-y + xc, -x + yc);
            make_coordinate(-x + xc, y + yc);
            make_coordinate(y + xc, -x + yc);

            if (p < 0)
            {
                x++;
                p = p + 2 * x + 1;
            }
            else
            {
                x++;
                y--;
                p = p + 2 * x - 2 * y + 1;
            }
        }
    }
};

#endif
