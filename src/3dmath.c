#include <stdio.h>
#include <assert.h>

#include "3dmath.h"


int calculate_model_view_matrix(
        mat4f *mvm, vec3f eye, vec3f center, vec3f up) {
    assert(mvm != NULL);

    // Create a new matrix that will hold the modelview matrix
    mtrix4fe mv = new Matrix4fe();

        // Set the identity matrix
        mv.setIdentity();

        // Calculate the forward vector
        Vector3f forward = new Vector3f();
        forward.sub(center, eye);
        forward.normalize();

        // Calculate the side vector by calculating the cross product of the
        // forward and up vector
        Vector3f side = new Vector3f();
        side.cross(forward, up);
        side.normalize();

        // Recalculate up
        up.cross(side, forward);

        // Set the matrix fields
        mv.setElement(0, 0, side.getX());
        mv.setElement(0, 1, side.getY());
        mv.setElement(0, 2, side.getZ());

        mv.setElement(1, 0, up.getX());
        mv.setElement(1, 1, up.getY());
        mv.setElement(1, 2, up.getZ());

        mv.setElement(2, 0, -forward.getX());
        mv.setElement(2, 1, -forward.getY());
        mv.setElement(2, 2, -forward.getZ());

        // Negate the eye vector and create a translation matrix
        eye.negate();
        Matrix4f translationMatrix = new Matrix4f();
        translationMatrix.setIdentity();
        translationMatrix.setTranslation(eye);

        // Apply the translation
        mv.mul(translationMatrix);

        return mv.getFloatArray();at4f_set_identity_matrix(&mvm);

    // Calculate the forward vector
    vec3f forward;
    vec3f_subtract(&forward, center, eye);

    //forward.normalize();
    vec3f_normalize(&forward);

    // Calculate the side vector by calculating the cross product of the
    // forward and up vector
    vec3f side;
    vec3f_cross(&side, forward, up);
    vec3f_normalize(&side);

    // Recalculate up
    vec3f_cross(&up, side, forward);

    // Set the matrix fields
    /*
    mv.setElement(0, 0, side.getX());
    mv.setElement(0, 1, side.getY());
    mv.setElement(0, 2, side.getZ());

    mv.setElement(1, 0, up.getX());
    mv.setElement(1, 1, up.getY());
    mv.setElement(1, 2, up.getZ());

    mv.setElement(2, 0, -forward.getX());
    mv.setElement(2, 1, -forward.getY());
    mv.setElement(2, 2, -forward.getZ());
    */

    // Negate the eye vector and create a translation matrix
    vec3f_negate(&eye);

    mat4f translation_matrix;
    mat4f_set_identity_matrix(&translation_matrix);
    //translationMatrix.setTranslation(eye);
    mat4f_add_vec3f(&translation_matrix, translation_matrix, eye);

    // Apply the translation
    mat4f_mult(mvm, *mvm, translation_matrix);

    return 1;
}
