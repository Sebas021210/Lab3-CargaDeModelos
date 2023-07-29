#pragma once

class Color {
public:
    Color() = default;
    Color(int red, int green, int blue);

    int getRed() const;
    int getGreen() const;
    int getBlue() const;

    void setRed(int red);
    void setGreen(int green);
    void setBlue(int blue);

private:
    int r;
    int g;
    int b;
};

Color::Color(int red, int green, int blue) {
    setRed(red);
    setGreen(green);
    setBlue(blue);
}

int Color::getRed() const {
    return r;
}

int Color::getGreen() const {
    return g;
}

int Color::getBlue() const {
    return b;
}

void Color::setRed(int red) {
    r = (red >= 0 && red <= 255) ? red : 0;
}

void Color::setGreen(int green) {
    g = (green >= 0 && green <= 255) ? green : 0;
}

void Color::setBlue(int blue) {
    b = (blue >= 0 && blue <= 255) ? blue : 0;
}
