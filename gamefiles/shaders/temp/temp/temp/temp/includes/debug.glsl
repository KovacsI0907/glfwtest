// Declare the global zero variable
bool zero = false;
// Declare the global NaN flag
bool nanFound = false;

// Function to check if a float is zero
void checkZero(float f) {
    if (f == 0.0) {
        zero = true;
    }
}

// Function to check if all components of a vec3 are zero
void checkZero(vec3 v) {
    if (v.x == 0.0 && v.y == 0.0 && v.z == 0.0) {
        zero = true;
    }
}

// Function to check if all components of a vec4 are zero
void checkZero(vec4 v) {
    if (v.x == 0.0 && v.y == 0.0 && v.z == 0.0 && v.w == 0.0) {
        zero = true;
    }
}

// Function to check if a float is NaN
void checkNan(float f) {
    if (f != f) { // NaN is the only value that is not equal to itself.
        nanFound = true;
    }
}

// Function to check if any component of a vec3 is NaN
void checkNan(vec3 v) {
    if (v.x != v.x || v.y != v.y || v.z != v.z) {
        nanFound = true;
    }
}

// Function to check if any component of a vec4 is NaN
void checkNan(vec4 v) {
    if (v.x != v.x || v.y != v.y || v.z != v.z || v.w != v.w) {
        nanFound = true;
    }
}