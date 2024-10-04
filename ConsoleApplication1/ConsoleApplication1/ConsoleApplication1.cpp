#include <iostream>
#include <string>
#include <vector>
#include <cmath>

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 80;
const int FIGURE_SCALE = 2;

struct Board {
    std::vector<std::vector<char>> grid;
    Board() : grid(BOARD_HEIGHT, std::vector<char>(BOARD_WIDTH, ' ')) {}

    void print() {
        for (auto& row : grid) {
            for (char c : row) {
                std::cout << c;
            }
            std::cout << "\n";
        }
    }

    void clear() {
        grid.assign(BOARD_HEIGHT, std::vector<char>(BOARD_WIDTH, ' '));
    }
};

struct Shape {
    virtual void draw(Board& board, int x, int y) = 0;
    virtual ~Shape() = default;
};

struct Circle : public Shape {
    int radius;

    Circle(int r) : radius(r) {}

    void draw(Board& board, int X, int Y) override {
        if (radius <= 0) return;

        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                float correctY = y * FIGURE_SCALE;
                float distance = sqrt(x * x + correctY * correctY);
                if (std::abs(distance - radius) <= 0.5) {
                    int drawnX = X + x;
                    int drawnY = Y + y;
                    if (drawnX >= 0 && drawnX < BOARD_WIDTH && drawnY >= 0 && drawnY < BOARD_HEIGHT) {
                        board.grid[drawnY][drawnX] = '*';
                    }
                }
            }
        }
    }
};

struct Square : public Shape {
    int side_length;

    Square(int side) : side_length(side) {}

    void draw(Board& board, int X, int Y) override {
        if (side_length <= 0) return;

        for (int y = 0; y < side_length; ++y) {
            float correctY = y / FIGURE_SCALE;

            for (int x = 0; x < side_length; ++x) {
                if (y == 0 || y == side_length - 1 || x == 0 || x == side_length - 1) {
                    int drawnX = X + x;
                    int drawnY = Y + static_cast<int>(correctY);
                    if (drawnX >= 0 && drawnX < BOARD_WIDTH && drawnY >= 0 && drawnY < BOARD_HEIGHT) {
                        board.grid[drawnY][drawnX] = '*';
                    }
                }
            }
        }
    }
};

struct Triangle : public Shape {
    int height;

    Triangle(int h) : height(h) {}

    void draw(Board& board, int x, int y) override {
        if (height <= 0) return;
        for (int i = 0; i < height; ++i) {
            int left = x - i;
            int right = x + i;
            int posY = y + i;

            if (posY < 25) {
                if (left >= 0 && left < 80)
                    board.grid[posY][left] = '*';
                if (right >= 0 && right < 80 && left != right)
                    board.grid[posY][right] = '*';
            }
        }
        for (int j = 0; j < 2 * height - 1; ++j) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < 80 && baseY < 25)
                board.grid[baseY][baseX] = '*';
        }
    }
};

int main() {
    Board board;
    std::string command;

    while (true) {
        std::cout << "> ";
        std::cin >> command;

        if (command == "draw") {
            board.print();
        }
        else if (command == "triangle") {
            int x, y, height;
            std::cout << "Enter the location of the triangle, and its height";
            std::cin >> x >> y >> height;

            Triangle triangle(height);
            triangle.draw(board, x, y);
        }
        else if (command == "circle") {
            int x, y, radius;
            std::cout << "Enter the location of the circle, and its radius: ";
            std::cin >> x >> y >> radius;

            Circle circle(radius);
            circle.draw(board, x, y);
        }
        else if (command == "square") {
            int x, y, side_length;
            std::cout << "Enter the location of the square, and its side length: ";
            std::cin >> x >> y >> side_length;

            Square square(side_length);
            square.draw(board, x, y);
        }
        else if (command == "list") {
            std::cout << "circle coordinates radius\n";
            std::cout << "square coordinates side size\n";
            std::cout << "triangle coordinates height\n";
        }
        else if (command == "exit") {
            break;
        }
    }

    return 0;
}
