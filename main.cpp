#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <vector>
#include <ctime>
#include <string>
#include <iostream> // Include for std::cerr

int screenWidth;
int screenHeight;

const int gridSize = 20;
int snakeLength = 3;
int score = 0;

enum Direction { UP, DOWN, LEFT, RIGHT };
Direction dir = RIGHT;

struct Point {
    int x, y;
};

std::vector<Point> snake;
Point food;

bool gameOver = false;
bool isPaused = false;

GLuint boardTexture;

void initOpenGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); // Enable textures

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    // Load board texture
    boardTexture = SOIL_load_OGL_texture(
        "board.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (boardTexture == 0) {
        std::cerr << "Texture loading failed: " << SOIL_last_result() << std::endl;
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void spawnFood() {
    srand(time(0));
    food.x = rand() % gridSize;
    food.y = rand() % gridSize;
}

void initGame() {
    snake.clear();
    for (int i = 0; i < snakeLength; i++) {
        snake.push_back({5 - i, 5});
    }
    spawnFood();
    score = 0;
    gameOver = false;
    isPaused = false;
    dir = RIGHT;
    glutPostRedisplay();
}

void drawText(float x, float y, std::string text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawCube(float x, float y, float z, float size) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidCube(size);
    glPopMatrix();
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(10.0, 25.0, 25.0, 10.0, 0.0, 10.0, 0.0, 1.0, 0.0);

    // 🎯 Draw Board with texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, boardTexture);

    float minX = -0.5f;
    float maxX = gridSize - 0.5f;
    float minZ = -0.5f;
    float maxZ = gridSize - 0.5f;

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(minX, 0.0f, minZ);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(maxX, 0.0f, minZ);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(maxX, 0.0f, maxZ);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(minX, 0.0f, maxZ);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Elevate cubes to avoid overlap with board
    float cubeHeight = 0.45f;
    float elevation = cubeHeight + 0.01f;

    // 🐍 Draw Snake (Vibrant Green)
    glColor3f(0.2f, 0.8f, 0.2f);  // Vibrant green for snake
    for (auto& p : snake)
        drawCube(p.x, elevation, p.y, 0.9);

    // 🍎 Draw Food (Bright Red)
    glColor3f(1.0f, 0.3f, 0.3f); // Bright red color
    drawCube(food.x, elevation, food.y, 0.9);

    // 🧮 Draw Score
    std::string scoreText = "Score: " + std::to_string(score);
    glColor3f(1.0f, 1.0f, 0.5f); // Light Yellow for score
    drawText(10, screenHeight - 30, scoreText);

    // 🛑 Game Over Text
    if (gameOver) {
        drawText(screenWidth / 2 - 140, screenHeight / 2, "Game Over! Press S to start");
    }

    // ⏸ Pause Text
    if (isPaused) {
        drawText(screenWidth / 2 - 140, screenHeight / 2, "Game Paused! Press P to resume");
    }

    glutSwapBuffers();
}

void updateSnake() {
    if (gameOver || isPaused) return;

    Point newHead = snake[0];
    switch (dir) {
        case UP: newHead.y++; break;
        case DOWN: newHead.y--; break;
        case LEFT: newHead.x--; break;
        case RIGHT: newHead.x++; break;
    }

    if (newHead.x < 0 || newHead.y < 0 || newHead.x >= gridSize || newHead.y >= gridSize)
        gameOver = true;

    for (auto& part : snake)
        if (part.x == newHead.x && part.y == newHead.y)
            gameOver = true;

    snake.insert(snake.begin(), newHead);

    if (newHead.x == food.x && newHead.y == food.y) {
        spawnFood();
        score += 10;
    } else {
        snake.pop_back();
    }

    glutPostRedisplay();
}

void timer(int = 0) {
    if (!gameOver && !isPaused) {
        updateSnake();
    }
    glutTimerFunc(200, timer, 0);
}

void keyboard(int key, int, int) {
    switch (key) {
        case GLUT_KEY_UP: if (dir != UP) dir = DOWN; break;
        case GLUT_KEY_DOWN: if (dir != DOWN) dir = UP; break;
        case GLUT_KEY_LEFT: if (dir != RIGHT) dir = LEFT; break;
        case GLUT_KEY_RIGHT: if (dir != LEFT) dir = RIGHT; break;
    }
}

void normalKey(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'p':
            isPaused = !isPaused;
            glutPostRedisplay();
            break;
        case 's':
            if (gameOver) {
                initGame();
            }
            break;
    }
}

void reshape(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(screenWidth / 2, screenHeight / 2);
    glutCreateWindow("Snake Game");

    initOpenGL();
    initGame();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(normalKey);
    timer();

    glutMainLoop();
    return 0;
}
