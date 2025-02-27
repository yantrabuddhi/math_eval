#include <raylib.h>
#include <cmath> // for std::sqrt
#include <vector>
#include <iostream>
#include <def_math.h>

// Function to map a value from one range to another
float mapValue(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
    return outputMin + ((value - inputMin) / (inputMax - inputMin)) * (outputMax - outputMin);
}

// Function to compute f(x) = sqrt(x^2 + 4)
double fn(double x, const std::shared_ptr<meval::var_map>& vars,const meval::math_expr& expr) {
    (*vars)["x"] = x;
    return expr.eval();
}

int main() {
    /* Math eval
     */
    std::string s{"($x^2+4)^(1/2)"};
    //std::string s{"@sin($x)"};
    std::shared_ptr<meval::var_map> vars = std::make_shared<meval::var_map>();
    std::shared_ptr<meval::func_map> funcs = std::make_shared<meval::func_map>();
    std::shared_ptr<meval::operator_map> ops = std::make_shared<meval::operator_map>();
    meval::init_def_vars(vars);
    meval::init_def_funcs(funcs);
    meval::init_def_ops(ops);
    meval::math_expr mexp(s, vars, funcs, ops);

    // Initialize the screen dimensions
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Define the graph range for x and y
    const float xMin = -10.0f;
    const float xMax = 10.0f;

    const float yMin = 0.0f; // Minimum y value; since the function sqrt(x^2 + 4) >= 2
    const float yMax = 15.0f;

    // Initialize the raylib window
    InitWindow(screenWidth, screenHeight, "Graph of sqrt(x^2 + 4)");

    // Set the graph origin to the middle of the screen
    Vector2 origin = { screenWidth / 2.0f, screenHeight / 2.0f };

    // Set the step size for x values
    const float step = 0.1f;

    std::vector<Vector2> points;
    try {
        // Calculate the points for the graph
        for (float x = xMin; x <= xMax; x += step) {

            float y = fn(x,vars,mexp);
            std::cout << x << " " << y << std::endl;


            // Map x and y from world coordinates to screen coordinates
            float screenX = mapValue(x, xMin, xMax, 0, screenWidth);
            float screenY = mapValue(y, yMin, yMax, screenHeight, 0); // Y is inverted (top is 0)

            points.push_back({ screenX, screenY });
        }
    }catch (const meval::meval_error& merr) {
        std::cout << merr.what() << std::endl;
    }
    catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }

    // Main game loop
    while (!WindowShouldClose()) {
        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the axes
        DrawLine(0, origin.y, screenWidth, origin.y, GRAY); // X-axis
        DrawLine(origin.x, 0, origin.x, screenHeight, GRAY); // Y-axis

        // Draw the graph
        for (size_t i = 0; i < points.size() - 1; ++i) {
            DrawLineV(points[i], points[i + 1], BLUE);
        }

        // Add labels
        DrawText(("y = "+s).c_str(), 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    // Close the raylib window
    CloseWindow();

    return 0;
}