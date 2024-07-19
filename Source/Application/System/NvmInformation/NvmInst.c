///////////////////////////////////////////////////////////
//  NvmInst.c
//  Implementation of the NVM Data
//  Created on:      2016/05/18 10:09:23
//  Original author: Auto Generate By Excel
///////////////////////////////////////////////////////////

#include "NvmInst.h"
#include "NvmRam.h"
#include "ENvmID.h"


static const CNvmNvmSpec nvmid_finisher_speed = { &(nvmRamArea[0]), 763, 1, 0, 3, 0, 0, 0, 0, 0, 0, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_finisher_major_version = { &(nvmRamArea[4]), 763, 2, 0, 65535, 0, 0, 0, 0, 0, 0, 0, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_finisher_minor_version = { &(nvmRamArea[6]), 763, 3, 0, 65535, 0, 0, 0, 0, 0, 0, 0, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_finisher_patch_version = { &(nvmRamArea[8]), 763, 4, 0, 65535, 0, 0, 0, 0, 0, 0, 0, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_nvm_ipl_version = { &(nvmRamArea[10]), 763, 5, 0, 65535, 0, 0, 0, 0, 0, 0, 0, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_version_down_init_enable = { &(nvmRamArea[1]), 763, 6, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_iot_exit_sensor_active_adjust = { &(nvmRamArea[2]), 763, 7, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_uncoat_80_staple_50 = { &(nvmRamArea[3]), 763, 8, 25, 75, 50, 50, 50, 50, 50, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_uncoat_90_staple_50 = { &(nvmRamArea[12]), 763, 9, 25, 75, 50, 50, 50, 50, 50, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_uncoat_105_staple_50 = { &(nvmRamArea[13]), 763, 10, 2, 75, 30, 30, 30, 30, 30, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_uncoat_128_staple_50 = { &(nvmRamArea[14]), 763, 11, 2, 75, 25, 25, 25, 25, 25, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_uncoat_150_staple_50 = { &(nvmRamArea[15]), 763, 12, 2, 75, 20, 20, 20, 20, 20, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_uncoat_176_staple_50 = { &(nvmRamArea[16]), 763, 13, 2, 75, 15, 15, 15, 15, 15, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_uncoat_220_staple_50 = { &(nvmRamArea[17]), 763, 14, 2, 75, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_coat_128_staple_50 = { &(nvmRamArea[18]), 763, 15, 2, 75, 15, 15, 15, 15, 15, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_coat_150_staple_50 = { &(nvmRamArea[19]), 763, 16, 2, 75, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_coat_176_staple_50 = { &(nvmRamArea[20]), 763, 17, 2, 75, 7, 7, 7, 7, 7, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_sheet_coat_220_staple_50 = { &(nvmRamArea[21]), 763, 18, 2, 75, 5, 5, 5, 5, 5, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_set_pitch_setting1 = { &(nvmRamArea[24]), 763, 19, (UL)-200, 200, (UL)-200, (UL)-200, (UL)-200, (UL)-200, (UL)-200, 1, 1, 1, eCdiFinDataType_SignShort, };
static const CNvmNvmSpec nvmid_set_pitch_setting2 = { &(nvmRamArea[26]), 763, 20, (UL)-200, 200, (UL)-200, (UL)-200, (UL)-200, (UL)-200, (UL)-200, 1, 1, 1, eCdiFinDataType_SignShort, };
static const CNvmNvmSpec nvmid_read_flashdata_log_num = { &(nvmRamArea[22]), 763, 21, 0, 127, 1, 1, 1, 1, 1, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_serial_log_select = { &(nvmRamArea[28]), 763, 22, 0, 16383, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_transition_state = { &(nvmRamArea[23]), 763, 23, 0, 13, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_waiting_module_transition_state = { &(nvmRamArea[30]), 763, 24, 0, 65535, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_action_sequence_control = { &(nvmRamArea[32]), 763, 25, 0, 255, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_cause_response_unknown_preparation_time = { &(nvmRamArea[36]), 763, 26, 0, 4294967295, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignLong, };
static const CNvmNvmSpec nvmid_transport_mot_speed_v1 = { &(nvmRamArea[40]), 763, 200, 150, 410, 285, 285, 285, 285, 285, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_transport_mot_speed_v2 = { &(nvmRamArea[42]), 763, 201, 150, 410, 285, 285, 285, 285, 285, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_transport_mot_down_start_t1_1 = { &(nvmRamArea[33]), 763, 202, 0, 50, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_solenoid_push_time_a = { &(nvmRamArea[34]), 763, 203, 1, 20, 3, 3, 3, 3, 3, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_solenoid_pull_time_b = { &(nvmRamArea[35]), 763, 204, 1, 20, 3, 3, 3, 3, 3, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_solenoid_count_c = { &(nvmRamArea[44]), 763, 205, 0, 100, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_num_staple_set = { &(nvmRamArea[45]), 763, 350, 0, 200, 0, 0, 0, 0, 0, 1, 0, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sheet_width_last_ejected_sheet = { &(nvmRamArea[48]), 763, 351, 0, 65535, 0, 0, 0, 0, 0, 1, 0, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_sheet_length_last_ejected_sheet = { &(nvmRamArea[50]), 763, 352, 0, 65535, 0, 0, 0, 0, 0, 1, 0, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_sheet_width_maximum_size_sheet = { &(nvmRamArea[52]), 763, 353, 0, 65535, 0, 0, 0, 0, 0, 1, 0, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_sheet_length_maximum_size_sheet = { &(nvmRamArea[54]), 763, 354, 0, 65535, 0, 0, 0, 0, 0, 1, 0, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_mix_stack = { &(nvmRamArea[46]), 763, 355, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_stack_paper_limit = { &(nvmRamArea[56]), 763, 356, 0, 4294967295, 0, 0, 0, 0, 0, 0, 1, 1, eCdiFinDataType_UnsignLong, };
static const CNvmNvmSpec nvmid_stacker_mot_up_count = { &(nvmRamArea[60]), 763, 357, 0, 4294967295, 0, 0, 0, 0, 0, 0, 0, 1, eCdiFinDataType_UnsignLong, };
static const CNvmNvmSpec nvmid_stacker_mot_down_count = { &(nvmRamArea[64]), 763, 358, 0, 4294967295, 0, 0, 0, 0, 0, 0, 0, 1, eCdiFinDataType_UnsignLong, };
static const CNvmNvmSpec nvmid_elevator_mot_pulse_count = { &(nvmRamArea[68]), 763, 359, 0, 90000, 0, 0, 0, 0, 0, 0, 0, 1, eCdiFinDataType_UnsignLong, };
static const CNvmNvmSpec nvmid_low_pulse_limit = { &(nvmRamArea[47]), 763, 360, 1, 250, 180, 180, 180, 180, 180, 0, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_mix_stack_enable = { &(nvmRamArea[72]), 763, 361, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_full_stack_off_count = { &(nvmRamArea[76]), 763, 362, 0, 500, 149, 149, 149, 149, 149, 0, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_mix_full_stack_pos_pulse_limit = { &(nvmRamArea[78]), 763, 363, 500, 4000, 756, 756, 756, 756, 756, 0, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_l_full_stack_pos_pulse_limit = { &(nvmRamArea[80]), 763, 364, 1000, 4000, 1512, 1512, 1512, 1512, 1512, 0, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_s_full_stack_pos_pulse_limit = { &(nvmRamArea[82]), 763, 365, 1000, 4000, 1632, 1632, 1632, 1632, 1632, 0, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_full_stack_position1 = { &(nvmRamArea[73]), 763, 366, 0, 250, 59, 59, 59, 59, 59, 0, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_full_stack_position2 = { &(nvmRamArea[84]), 763, 367, 0, 2000, 500, 500, 500, 500, 500, 0, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_corner_staple_count_n1 = { &(nvmRamArea[74]), 763, 368, 0, 255, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_corner_staple_mode_count_n2 = { &(nvmRamArea[75]), 763, 369, 1, 255, 65, 65, 65, 65, 65, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_stacker_tray_down_un_staple_timming_t1 = { &(nvmRamArea[88]), 763, 370, 0, 120, 110, 110, 110, 110, 110, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_compile_position_h3 = { &(nvmRamArea[89]), 763, 371, 0, 50, 25, 25, 25, 25, 25, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_up_count_g1 = { &(nvmRamArea[90]), 763, 372, 0, 250, 35, 35, 35, 35, 35, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_up_count_g2 = { &(nvmRamArea[91]), 763, 373, 0, 250, 90, 90, 90, 90, 90, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_un_staple_stacker_down_timing_at_eject = { &(nvmRamArea[92]), 763, 374, 0, 50, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_corner_staple_stacker_down_timing_at_eject = { &(nvmRamArea[93]), 763, 375, 0, 50, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_down_count_h1_1 = { &(nvmRamArea[86]), 763, 376, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_down_count_h1_2 = { &(nvmRamArea[96]), 763, 377, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_down_count_h2_1 = { &(nvmRamArea[98]), 763, 378, 0, 450, 300, 300, 300, 300, 300, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_down_count_h2_2 = { &(nvmRamArea[100]), 763, 379, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stack_lift_downcount_h1_3_nouse = { &(nvmRamArea[102]), 763, 380, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stack_lift_downcoun_h2_3_nouse = { &(nvmRamArea[104]), 763, 381, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_s_corner_staple_stack_set_limit = { &(nvmRamArea[94]), 763, 382, 1, 150, 60, 60, 60, 60, 60, 0, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_l_corner_staple_stack_set_limit = { &(nvmRamArea[95]), 763, 383, 1, 150, 60, 60, 60, 60, 60, 0, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_num_set_staple_position = { &(nvmRamArea[108]), 763, 384, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_compile_sheet_count_for_staple_50 = { &(nvmRamArea[109]), 763, 400, 25, 75, 50, 50, 50, 50, 50, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_last_offset_position = { &(nvmRamArea[110]), 763, 401, 0, 255, 0, 0, 0, 0, 0, 1, 0, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_compile_sheet_for_unstaple_size_s = { &(nvmRamArea[111]), 763, 402, 10, 50, 20, 20, 20, 20, 20, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_max_compile_sheet_for_unstaple_size_l = { &(nvmRamArea[112]), 763, 403, 5, 50, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamping_start_setting1 = { &(nvmRamArea[106]), 763, 404, 250, 500, 320, 320, 320, 320, 320, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_tamping_start_setting2 = { &(nvmRamArea[116]), 763, 405, 250, 500, 320, 320, 320, 320, 320, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_tamping_start_setting3 = { &(nvmRamArea[118]), 763, 406, 250, 1000, 500, 500, 500, 500, 500, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_tamping_start_setting4 = { &(nvmRamArea[120]), 763, 407, 250, 1000, 500, 500, 500, 500, 500, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_tamping_start_setting5 = { &(nvmRamArea[122]), 763, 408, 250, 1000, 500, 500, 500, 500, 500, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_tamping_start_setting6 = { &(nvmRamArea[124]), 763, 409, 250, 1000, 500, 500, 500, 500, 500, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_setlast_tamping_start_setting1 = { &(nvmRamArea[126]), 763, 410, 250, 1000, 320, 320, 320, 320, 320, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_setlast_tamping_start_setting2 = { &(nvmRamArea[128]), 763, 411, 250, 1000, 320, 320, 320, 320, 320, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_setlast_tamping_start_setting3 = { &(nvmRamArea[130]), 763, 412, 250, 1000, 400, 400, 400, 400, 400, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_setlast_tamping_start_setting4 = { &(nvmRamArea[132]), 763, 413, 250, 1000, 400, 400, 400, 400, 400, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_setlast_tamping_start_setting5 = { &(nvmRamArea[134]), 763, 414, 250, 1000, 400, 400, 400, 400, 400, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_setlast_tamping_start_setting6 = { &(nvmRamArea[136]), 763, 415, 250, 1000, 400, 400, 400, 400, 400, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_setlast_tamping_num1 = { &(nvmRamArea[113]), 763, 416, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_setlast_tamping_num2 = { &(nvmRamArea[114]), 763, 417, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamper_return_out_start_time = { &(nvmRamArea[115]), 763, 418, 0, 50, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamper_return_in_start_time = { &(nvmRamArea[140]), 763, 419, (UL)-10, 30, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_SignChar, };
static const CNvmNvmSpec nvmid_tamper_un_stple_adjst = { &(nvmRamArea[141]), 763, 420, (UL)-10, 15, (UL)-2, (UL)-2, (UL)-2, (UL)-2, (UL)-2, 1, 1, 1, eCdiFinDataType_SignChar, };
static const CNvmNvmSpec nvmid_tamper_r_stple_adjst = { &(nvmRamArea[142]), 763, 421, (UL)-10, 15, (UL)-4, (UL)-4, (UL)-4, (UL)-4, (UL)-4, 1, 1, 1, eCdiFinDataType_SignChar, };
static const CNvmNvmSpec nvmid_tamping_add_push_pulse = { &(nvmRamArea[143]), 763, 422, (UL)-10, 10, 3, 3, 3, 3, 3, 1, 1, 1, eCdiFinDataType_SignChar, };
static const CNvmNvmSpec nvmid_tamping_add_push_paper_count = { &(nvmRamArea[144]), 763, 423, 0, 75, 41, 41, 41, 41, 41, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_staple_count = { &(nvmRamArea[148]), 763, 424, 0, 65533000, 0, 0, 0, 0, 0, 0, 1, 1, eCdiFinDataType_UnsignLong, };
static const CNvmNvmSpec nvmid_stapler_mot_pwm_low_setting1_1 = { &(nvmRamArea[138]), 763, 425, 1, 933, 200, 200, 200, 200, 200, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_staple_speed_change = { &(nvmRamArea[145]), 763, 426, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_staple_coat_change = { &(nvmRamArea[146]), 763, 427, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_staple_count_detect_near_low_staple = { &(nvmRamArea[152]), 763, 428, 0, 5100, 0, 0, 0, 0, 0, 0, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_detect_near_low_staple_threshold = { &(nvmRamArea[154]), 763, 429, 4500, 5000, 4500, 4500, 4500, 4500, 4500, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_sub_paddle_stop_time_1 = { &(nvmRamArea[147]), 763, 430, 0, 10, 8, 8, 8, 8, 8, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_eject_speed_change_time_2 = { &(nvmRamArea[156]), 763, 431, 0, 2000, 450, 450, 450, 450, 450, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_eject_compile_motor_stop_pulse = { &(nvmRamArea[160]), 763, 432, 0, 100, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_set_clamp_up_start_time_4 = { &(nvmRamArea[158]), 763, 433, 400, 700, 500, 500, 500, 500, 500, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_set_clamp_up_stop_time_5 = { &(nvmRamArea[161]), 763, 434, 0, 100, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_set_clamp_lower_start_time_6 = { &(nvmRamArea[162]), 763, 435, 0, 40, 26, 26, 26, 26, 26, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_set_clamp_lower_stop_time_7 = { &(nvmRamArea[163]), 763, 436, 0, 200, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_start_time_8_1 = { &(nvmRamArea[164]), 763, 437, 0, 250, 120, 120, 120, 120, 120, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_start_time_8_2 = { &(nvmRamArea[165]), 763, 438, 0, 250, 120, 120, 120, 120, 120, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_start_time_8_3 = { &(nvmRamArea[166]), 763, 439, 0, 250, 120, 120, 120, 120, 120, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_profile_type_1 = { &(nvmRamArea[167]), 763, 440, 1, 3, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_profile_type_2 = { &(nvmRamArea[168]), 763, 441, 1, 3, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_profile_type_3 = { &(nvmRamArea[169]), 763, 442, 1, 3, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_count1 = { &(nvmRamArea[170]), 763, 443, 1, 10, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_count2 = { &(nvmRamArea[171]), 763, 444, 1, 10, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_count3 = { &(nvmRamArea[172]), 763, 445, 1, 10, 5, 5, 5, 5, 5, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_count4 = { &(nvmRamArea[173]), 763, 446, 1, 10, 5, 5, 5, 5, 5, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_count5 = { &(nvmRamArea[174]), 763, 447, 1, 10, 5, 5, 5, 5, 5, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_count6 = { &(nvmRamArea[175]), 763, 448, 1, 10, 5, 5, 5, 5, 5, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_change_count_n = { &(nvmRamArea[176]), 763, 449, 1, 75, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_stapler_mot_pwm_low_setting6_20 = { &(nvmRamArea[180]), 763, 450, 1, 933, 200, 200, 200, 200, 200, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stapler_mot_pwm_low_setting21_40 = { &(nvmRamArea[182]), 763, 451, 1, 933, 200, 200, 200, 200, 200, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_sub_paddle_change_count_n_2 = { &(nvmRamArea[177]), 763, 452, 1, 75, 21, 21, 21, 21, 21, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_change_count_n_3 = { &(nvmRamArea[178]), 763, 453, 1, 75, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_down_count_h1_3 = { &(nvmRamArea[184]), 763, 454, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_down_count_h1_4 = { &(nvmRamArea[186]), 763, 455, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_down_count_h2_3 = { &(nvmRamArea[188]), 763, 456, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_stacker_mot_lift_down_count_h2_4 = { &(nvmRamArea[190]), 763, 457, 0, 450, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_paper_set_count_n = { &(nvmRamArea[179]), 763, 458, 0, 10, 5, 5, 5, 5, 5, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_mot_down_start_t1_2 = { &(nvmRamArea[192]), 763, 206, 0, 50, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamper_size_out_unstaple_st_t_adj = { &(nvmRamArea[193]), 763, 459, 2, 70, 65, 65, 65, 65, 65, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamper_size_out_staple_st_t_adj = { &(nvmRamArea[194]), 763, 460, 2, 70, 65, 65, 65, 65, 65, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamper_offset_push_pulse = { &(nvmRamArea[195]), 763, 461, 50, 80, 80, 80, 80, 80, 80, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_mot_down_start_t2_1 = { &(nvmRamArea[196]), 763, 207, 0, 250, 60, 60, 60, 60, 60, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_mot_down_start_t2_2 = { &(nvmRamArea[197]), 763, 208, 0, 250, 60, 60, 60, 60, 60, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_before_elevator_mot_pulse_count = { &(nvmRamArea[200]), 763, 385, 0, 90000, 0, 0, 0, 0, 0, 0, 0, 1, eCdiFinDataType_UnsignLong, };
static const CNvmNvmSpec nvmid_transport_mot_speed_v3 = { &(nvmRamArea[204]), 763, 209, 400, 850, 500, 500, 500, 500, 500, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_transport_mot_speed_v4 = { &(nvmRamArea[206]), 763, 210, 400, 850, 600, 600, 600, 600, 600, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_sub_paddle_change_count_p2 = { &(nvmRamArea[198]), 763, 462, 1, 3, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_sub_paddle_change_count_n_2_b5 = { &(nvmRamArea[199]), 763, 463, 1, 75, 2, 2, 2, 2, 2, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamping_add_push_pulse_2 = { &(nvmRamArea[208]), 763, 464, (UL)-10, 10, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_SignChar, };
static const CNvmNvmSpec nvmid_set_pitch_setting3 = { &(nvmRamArea[212]), 763, 465, (UL)-200, 200, (UL)-200, (UL)-200, (UL)-200, (UL)-200, (UL)-200, 1, 1, 1, eCdiFinDataType_SignShort, };
static const CNvmNvmSpec nvmid_transport_mot_speed_v5 = { &(nvmRamArea[214]), 763, 466, 400, 850, 700, 700, 700, 700, 700, 1, 1, 1, eCdiFinDataType_UnsignShort, };
static const CNvmNvmSpec nvmid_transport_mot_down_start_t1_3 = { &(nvmRamArea[209]), 763, 467, 0, 50, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_mot_down_start_t1_4 = { &(nvmRamArea[210]), 763, 468, 0, 50, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_mot_down_start_t2_3 = { &(nvmRamArea[211]), 763, 469, 0, 250, 60, 60, 60, 60, 60, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_transport_mot_down_start_t2_4 = { &(nvmRamArea[216]), 763, 470, 0, 250, 60, 60, 60, 60, 60, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamper_missregi_start_time = { &(nvmRamArea[217]), 763, 471, 0, 100, 3, 3, 3, 3, 3, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamper_missregi_mot_pulse = { &(nvmRamArea[218]), 763, 472, 0, 10, 3, 3, 3, 3, 3, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamping_setlast_add_pulse_adj_n = { &(nvmRamArea[219]), 763, 473, 0, 75, 21, 21, 21, 21, 21, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamping_setlast_add_pulse_5 = { &(nvmRamArea[220]), 763, 474, (UL)-10, 10, 3, 3, 3, 3, 3, 1, 1, 1, eCdiFinDataType_SignChar, };
static const CNvmNvmSpec nvmid_tamping_setlast_add_pulse_6 = { &(nvmRamArea[221]), 763, 475, (UL)-10, 10, 0, 0, 0, 0, 0, 1, 1, 1, eCdiFinDataType_SignChar, };
static const CNvmNvmSpec nvmid_tamping_normal_lt_curr_change_1 = { &(nvmRamArea[222]), 763, 476, 0, 100, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamping_normal_lt_curr_change_2 = { &(nvmRamArea[223]), 763, 477, 0, 100, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamping_setlast_lt_curr_change_1 = { &(nvmRamArea[224]), 763, 478, 0, 100, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };
static const CNvmNvmSpec nvmid_tamping_setlast_lt_curr_change_2 = { &(nvmRamArea[225]), 763, 479, 0, 100, 10, 10, 10, 10, 10, 1, 1, 1, eCdiFinDataType_UnsignChar, };

const CNvmNVM nvmAllData[NVMID_NUM] = {
    { &nvmid_finisher_speed },
    { &nvmid_finisher_major_version },
    { &nvmid_finisher_minor_version },
    { &nvmid_finisher_patch_version },
    { &nvmid_nvm_ipl_version },
    { &nvmid_version_down_init_enable },
    { &nvmid_iot_exit_sensor_active_adjust },
    { &nvmid_max_sheet_uncoat_80_staple_50 },
    { &nvmid_max_sheet_uncoat_90_staple_50 },
    { &nvmid_max_sheet_uncoat_105_staple_50 },
    { &nvmid_max_sheet_uncoat_128_staple_50 },
    { &nvmid_max_sheet_uncoat_150_staple_50 },
    { &nvmid_max_sheet_uncoat_176_staple_50 },
    { &nvmid_max_sheet_uncoat_220_staple_50 },
    { &nvmid_max_sheet_coat_128_staple_50 },
    { &nvmid_max_sheet_coat_150_staple_50 },
    { &nvmid_max_sheet_coat_176_staple_50 },
    { &nvmid_max_sheet_coat_220_staple_50 },
    { &nvmid_set_pitch_setting1 },
    { &nvmid_set_pitch_setting2 },
    { &nvmid_read_flashdata_log_num },
    { &nvmid_serial_log_select },
    { &nvmid_transition_state },
    { &nvmid_waiting_module_transition_state },
    { &nvmid_action_sequence_control },
    { &nvmid_cause_response_unknown_preparation_time },
    { &nvmid_transport_mot_speed_v1 },
    { &nvmid_transport_mot_speed_v2 },
    { &nvmid_transport_mot_down_start_t1_1 },
    { &nvmid_transport_solenoid_push_time_a },
    { &nvmid_transport_solenoid_pull_time_b },
    { &nvmid_transport_solenoid_count_c },
    { &nvmid_num_staple_set },
    { &nvmid_sheet_width_last_ejected_sheet },
    { &nvmid_sheet_length_last_ejected_sheet },
    { &nvmid_sheet_width_maximum_size_sheet },
    { &nvmid_sheet_length_maximum_size_sheet },
    { &nvmid_mix_stack },
    { &nvmid_stack_paper_limit },
    { &nvmid_stacker_mot_up_count },
    { &nvmid_stacker_mot_down_count },
    { &nvmid_elevator_mot_pulse_count },
    { &nvmid_low_pulse_limit },
    { &nvmid_mix_stack_enable },
    { &nvmid_full_stack_off_count },
    { &nvmid_mix_full_stack_pos_pulse_limit },
    { &nvmid_l_full_stack_pos_pulse_limit },
    { &nvmid_s_full_stack_pos_pulse_limit },
    { &nvmid_full_stack_position1 },
    { &nvmid_full_stack_position2 },
    { &nvmid_corner_staple_count_n1 },
    { &nvmid_corner_staple_mode_count_n2 },
    { &nvmid_stacker_tray_down_un_staple_timming_t1 },
    { &nvmid_compile_position_h3 },
    { &nvmid_stacker_mot_lift_up_count_g1 },
    { &nvmid_stacker_mot_lift_up_count_g2 },
    { &nvmid_un_staple_stacker_down_timing_at_eject },
    { &nvmid_corner_staple_stacker_down_timing_at_eject },
    { &nvmid_stacker_mot_lift_down_count_h1_1 },
    { &nvmid_stacker_mot_lift_down_count_h1_2 },
    { &nvmid_stacker_mot_lift_down_count_h2_1 },
    { &nvmid_stacker_mot_lift_down_count_h2_2 },
    { &nvmid_stack_lift_downcount_h1_3_nouse },
    { &nvmid_stack_lift_downcoun_h2_3_nouse },
    { &nvmid_s_corner_staple_stack_set_limit },
    { &nvmid_l_corner_staple_stack_set_limit },
    { &nvmid_num_set_staple_position },
    { &nvmid_max_compile_sheet_count_for_staple_50 },
    { &nvmid_last_offset_position },
    { &nvmid_max_compile_sheet_for_unstaple_size_s },
    { &nvmid_max_compile_sheet_for_unstaple_size_l },
    { &nvmid_tamping_start_setting1 },
    { &nvmid_tamping_start_setting2 },
    { &nvmid_tamping_start_setting3 },
    { &nvmid_tamping_start_setting4 },
    { &nvmid_tamping_start_setting5 },
    { &nvmid_tamping_start_setting6 },
    { &nvmid_setlast_tamping_start_setting1 },
    { &nvmid_setlast_tamping_start_setting2 },
    { &nvmid_setlast_tamping_start_setting3 },
    { &nvmid_setlast_tamping_start_setting4 },
    { &nvmid_setlast_tamping_start_setting5 },
    { &nvmid_setlast_tamping_start_setting6 },
    { &nvmid_setlast_tamping_num1 },
    { &nvmid_setlast_tamping_num2 },
    { &nvmid_tamper_return_out_start_time },
    { &nvmid_tamper_return_in_start_time },
    { &nvmid_tamper_un_stple_adjst },
    { &nvmid_tamper_r_stple_adjst },
    { &nvmid_tamping_add_push_pulse },
    { &nvmid_tamping_add_push_paper_count },
    { &nvmid_staple_count },
    { &nvmid_stapler_mot_pwm_low_setting1_1 },
    { &nvmid_staple_speed_change },
    { &nvmid_staple_coat_change },
    { &nvmid_staple_count_detect_near_low_staple },
    { &nvmid_detect_near_low_staple_threshold },
    { &nvmid_sub_paddle_stop_time_1 },
    { &nvmid_eject_speed_change_time_2 },
    { &nvmid_eject_compile_motor_stop_pulse },
    { &nvmid_set_clamp_up_start_time_4 },
    { &nvmid_set_clamp_up_stop_time_5 },
    { &nvmid_set_clamp_lower_start_time_6 },
    { &nvmid_set_clamp_lower_stop_time_7 },
    { &nvmid_sub_paddle_start_time_8_1 },
    { &nvmid_sub_paddle_start_time_8_2 },
    { &nvmid_sub_paddle_start_time_8_3 },
    { &nvmid_sub_paddle_profile_type_1 },
    { &nvmid_sub_paddle_profile_type_2 },
    { &nvmid_sub_paddle_profile_type_3 },
    { &nvmid_sub_paddle_count1 },
    { &nvmid_sub_paddle_count2 },
    { &nvmid_sub_paddle_count3 },
    { &nvmid_sub_paddle_count4 },
    { &nvmid_sub_paddle_count5 },
    { &nvmid_sub_paddle_count6 },
    { &nvmid_sub_paddle_change_count_n },
    { &nvmid_stapler_mot_pwm_low_setting6_20 },
    { &nvmid_stapler_mot_pwm_low_setting21_40 },
    { &nvmid_sub_paddle_change_count_n_2 },
    { &nvmid_sub_paddle_change_count_n_3 },
    { &nvmid_stacker_mot_lift_down_count_h1_3 },
    { &nvmid_stacker_mot_lift_down_count_h1_4 },
    { &nvmid_stacker_mot_lift_down_count_h2_3 },
    { &nvmid_stacker_mot_lift_down_count_h2_4 },
    { &nvmid_paper_set_count_n },
    { &nvmid_transport_mot_down_start_t1_2 },
    { &nvmid_tamper_size_out_unstaple_st_t_adj },
    { &nvmid_tamper_size_out_staple_st_t_adj },
    { &nvmid_tamper_offset_push_pulse },
    { &nvmid_transport_mot_down_start_t2_1 },
    { &nvmid_transport_mot_down_start_t2_2 },
    { &nvmid_before_elevator_mot_pulse_count },
    { &nvmid_transport_mot_speed_v3 },
    { &nvmid_transport_mot_speed_v4 },
    { &nvmid_sub_paddle_change_count_p2 },
    { &nvmid_sub_paddle_change_count_n_2_b5 },
    { &nvmid_tamping_add_push_pulse_2 },
    { &nvmid_set_pitch_setting3 },
    { &nvmid_transport_mot_speed_v5 },
    { &nvmid_transport_mot_down_start_t1_3 },
    { &nvmid_transport_mot_down_start_t1_4 },
    { &nvmid_transport_mot_down_start_t2_3 },
    { &nvmid_transport_mot_down_start_t2_4 },
    { &nvmid_tamper_missregi_start_time },
    { &nvmid_tamper_missregi_mot_pulse },
    { &nvmid_tamping_setlast_add_pulse_adj_n },
    { &nvmid_tamping_setlast_add_pulse_5 },
    { &nvmid_tamping_setlast_add_pulse_6 },
    { &nvmid_tamping_normal_lt_curr_change_1 },
    { &nvmid_tamping_normal_lt_curr_change_2 },
    { &nvmid_tamping_setlast_lt_curr_change_1 },
    { &nvmid_tamping_setlast_lt_curr_change_2 },
};
