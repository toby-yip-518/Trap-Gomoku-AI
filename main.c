#include <stdio.h>
#include <stdlib.h>
#include "battle_base.h"
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b)) 

// Helper function to evaluate a direction
void evaluate_direction(int x, int y, int dx, int dy, int piece, int states[MAX_M][MAX_N], int *consecutive, int *empty_ends, int *double_space_threat, int *twocon_space_threat) {
    *consecutive = 1;
    *empty_ends = 0;

    // Backward direction
    for (int back = 1; back <= 4; back++) {
        int nx = x - dx * back, ny = y - dy * back;
        if (nx < 0 || nx >= MAX_M || ny < 0 || ny >= MAX_N) break;
        if (states[nx][ny] == piece) {
            (*consecutive)++;
        } else if (states[nx][ny] == 0) {
            if (states[nx - dx][ny - dy] == piece) {
                if (states[nx - 2 * dx][ny - 2 * dy] == 0 && *consecutive >= 2) (*double_space_threat)++;
                if (states[nx - 2 * dx][ny - 2 * dy] == piece && *consecutive >= 2) (*twocon_space_threat)++;
            }
            (*empty_ends)++;
            break;
        } else {
            break;
        }
    }

    // Forward direction
    for (int forward = 1; forward <= 4; forward++) {
        int nx = x + dx * forward, ny = y + dy * forward;
        if (nx < 0 || nx >= MAX_M || ny < 0 || ny >= MAX_N) break;
        if (states[nx][ny] == piece) {
            (*consecutive)++;
        } else if (states[nx][ny] == 0) {
            if (states[nx + dx][ny + dy] == piece) {
                if (states[nx + 2 * dx][ny + 2 * dy] == 0 && *consecutive >= 2) (*double_space_threat)++;
                if (states[nx + 2 * dx][ny + 2 * dy] == piece && *consecutive >= 2) (*twocon_space_threat)++;
            }
            (*empty_ends)++;
            break;
        } else {
            break;
        }
    }
}

// Function to calculate score based on the evaluation
int calculate_score(int consecutive, int empty_ends, int double_space_threat, int twocon_space_threat, int *no_dead_ends, int *no_oneempty, int *no_twoempty, int *threat_counter, int *two_triple_threat, int is_offensive) {
    int score = 0;

    if (consecutive >= 5) {
        return is_offensive ? /*score*/ : /*score*/; // Winning condition
    } else if (consecutive == 4) {
        score += is_offensive ? /*score*/ : /*score*/;
    } else if (consecutive == 3) {
        score += is_offensive ? /*score*/ : /*score*/;
    } else if (consecutive == 2) {
        score += is_offensive ? /*score*/ : /*score*/;
    }

    if (empty_ends == 0) {
        (*no_dead_ends)++;
        if (consecutive == 4) score -= /*score*/;
        if (consecutive == 3) score -= /*score*/;
        if (consecutive == 2) score -= /*score*/;
    } else if (empty_ends == 1) {
        (*no_oneempty)++;
        if (consecutive == 4) { score += is_offensive ? /*score*/ : /*score*/; (*threat_counter++); }
        if (consecutive == 3) { score += is_offensive ? /*score*/ : /*score*/; (*threat_counter++); (*two_triple_threat)++; }
        if (consecutive == 2) score += is_offensive ? /*score*/ : /*score*/;
    } else if (empty_ends == 2) {
        (*no_twoempty)++;
        if (consecutive == 4) { score += is_offensive ? /*score*/ : /*score*/; (*threat_counter++); }
        if (consecutive == 3) { score += is_offensive ? /*score*/ : /*score*/; (*threat_counter++); (*two_triple_threat)++; }
        if (consecutive == 2) score += is_offensive ? /*score*/ : /*score*/;
    }

    return score;
}

void play_games(int step, int rival_decision_x, int rival_decision_y) {
    int states[MAX_M][MAX_N];
    read_ckbd(step - 1, states);

    int legal_decision[MAX_M * MAX_N][2];
    int legal_count = 0;

    for (int m = 0; m < MAX_M; m++) {
        for (int n = 0; n < MAX_N; n++) {
            if (states[m][n] == 0) {
                legal_decision[legal_count][0] = m;
                legal_decision[legal_count][1] = n;
                legal_count++;
            }
        }
    }

    int my_piece = (step % 2 == 1) ? 1 : 2;
    int opponent_piece = (step % 2 == 1) ? 2 : 1;

    int best_move[2] = {-1, -1};
    int best_score = /*score*/;

    for (int i = 0; i < legal_count; i++) {
        int x = legal_decision[i][0];
        int y = legal_decision[i][1];

        int defensive_score = 0;
        int offensive_score = 0;

        int no_dead_ends = 0, no_oneempty = 0, no_twoempty = 0;
        int double_space_threat = 0, twocon_space_threat = 0 ;
		int threat_counter = 0, two_triple_threat = 0;

        // Defensive checks
        int consecutive, empty_ends;
        evaluate_direction(x, y, 0, 1, opponent_piece, states, &consecutive, &empty_ends, &double_space_threat, &twocon_space_threat);
        defensive_score += calculate_score(consecutive, empty_ends, double_space_threat, twocon_space_threat, &no_dead_ends, &no_oneempty, &no_twoempty, &threat_counter, &two_triple_threat, 0);

        evaluate_direction(x, y, 1, 0, opponent_piece, states, &consecutive, &empty_ends, &double_space_threat, &twocon_space_threat);
        defensive_score += calculate_score(consecutive, empty_ends, double_space_threat, twocon_space_threat, &no_dead_ends, &no_oneempty, &no_twoempty, &threat_counter, &two_triple_threat, 0);

        evaluate_direction(x, y, 1, 1, opponent_piece, states, &consecutive, &empty_ends, &double_space_threat, &twocon_space_threat);
        defensive_score += calculate_score(consecutive, empty_ends, double_space_threat, twocon_space_threat, &no_dead_ends, &no_oneempty, &no_twoempty, &threat_counter, &two_triple_threat, 0);

        evaluate_direction(x, y, 1, -1, opponent_piece, states, &consecutive, &empty_ends, &double_space_threat, &twocon_space_threat);
        defensive_score += calculate_score(consecutive, empty_ends, double_space_threat, twocon_space_threat, &no_dead_ends, &no_oneempty, &no_twoempty, &threat_counter, &two_triple_threat, 0);
		
		if (no_dead_ends >= 3) { defensive_score -= /*score*/; }
		else if (no_dead_ends == 2) { defensive_score -= /*score*/; }
		else if (no_dead_ends <= 1 && step != 1) { defensive_score += /*score*/; }
		if (double_space_threat >= 2) { defensive_score += /*score*/; }
		else if (double_space_threat == 1) { defensive_score += /*score*/; }
		if (twocon_space_threat >= 2) { defensive_score += /*score*/; }
		else if (twocon_space_threat == 1) { defensive_score += /*score*/; }
		if (two_triple_threat >= 2) { defensive_score += /*score*/; }
	
		if (threat_counter >= 3) { defensive_score += /*score*/; }
		else if (threat_counter == 2) {defensive_score += /*score*/; }
		
		no_dead_ends = 0; threat_counter = 0; 
		double_space_threat = 0; twocon_space_threat = 0; 
		two_triple_threat = 0;
		
        // Offensive checks
        evaluate_direction(x, y, 0, 1, my_piece, states, &consecutive, &empty_ends, &double_space_threat, &twocon_space_threat);
        offensive_score += calculate_score(consecutive, empty_ends, double_space_threat, twocon_space_threat, &no_dead_ends, &no_oneempty, &no_twoempty, &threat_counter, &two_triple_threat, 1);

        evaluate_direction(x, y, 1, 0, my_piece, states, &consecutive, &empty_ends, &double_space_threat, &twocon_space_threat);
        offensive_score += calculate_score(consecutive, empty_ends, double_space_threat, twocon_space_threat, &no_dead_ends, &no_oneempty, &no_twoempty, &threat_counter, &two_triple_threat, 1);

        evaluate_direction(x, y, 1, 1, my_piece, states, &consecutive, &empty_ends, &double_space_threat, &twocon_space_threat);
        offensive_score += calculate_score(consecutive, empty_ends, double_space_threat, twocon_space_threat, &no_dead_ends, &no_oneempty, &no_twoempty, &threat_counter, &two_triple_threat, 1);

        evaluate_direction(x, y, 1, -1, my_piece, states, &consecutive, &empty_ends, &double_space_threat, &twocon_space_threat);
        offensive_score += calculate_score(consecutive, empty_ends, double_space_threat, twocon_space_threat, &no_dead_ends, &no_oneempty, &no_twoempty, &threat_counter, &two_triple_threat, 1);

		if (no_dead_ends >= 3) { offensive_score -= /*score*/; }
		else if (no_dead_ends == 2) { offensive_score -= /*score*/; }
		else if (no_dead_ends <= 1 && step != 1) { offensive_score += /*score*/; }
		if (double_space_threat >= 2) { offensive_score += /*score*/; }
		else if (double_space_threat == 1) { offensive_score += /*score*/; }
		if (twocon_space_threat >= 2) { offensive_score += /*score*/; }
		else if (twocon_space_threat == 1) { offensive_score += /*score*/; }
		if (two_triple_threat >= 2) { offensive_score += /*score*/; }
	
		if (threat_counter >= 3) { offensive_score += /*score*/; }
		else if (threat_counter == 2) {offensive_score += /*score*/; }

        // Center proximity bonus
        if (offensive_score == 0 && defensive_score == 0) {
            int center_distance = abs(x - 7) + abs(y - 7);
            offensive_score += (14 - center_distance);
        }

        if (my_piece == 1) {
            offensive_score *= 1.1;
        } else {
            defensive_score *= 1.1;
        }

        int current_score = offensive_score + defensive_score;
        if (current_score >= best_score) {
            best_score = current_score;
            best_move[0] = x;
            best_move[1] = y;
        }
    }

    // Final decision
    printf("BS:%d", best_score);
    save_decision(best_move[0], best_move[1]);
}