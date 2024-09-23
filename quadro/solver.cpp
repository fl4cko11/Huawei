#include "solver.h"
#include "in_out.h"

const double perc = 1e-5;

double diskr(coefs *coefs) {
    return coefs->b * coefs->b - 4 * coefs->a * coefs->c;
}

bool is_zero(double d){
    return ( (d <= perc) && (d >= (perc * (-1))) );
}

roots quadro_solution(coefs *coefs){
    roots roots = {NAN, NAN};
    if (is_zero(coefs->a)) {
        if (1 - is_zero(coefs->b)) {
            roots.root1 = -coefs->c / coefs->b;
        }
    }
    else {
        double d = diskr(coefs);
        if (d>=0) {
            double sqr_d = sqrt(d); //из-за точности дабла может в 0 уйти
            if (1 - is_zero(sqr_d)){
                roots.root1 = (-coefs->b + sqrt(d)) / (2 * coefs->a);
                roots.root2 = (-coefs->b - sqrt(d)) / (2 * coefs->a);
            } 
            else {
                roots.root1 = -coefs->b / (2 * coefs->a);
            }
        }
    }
    return roots;
}