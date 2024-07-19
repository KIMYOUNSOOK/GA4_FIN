///////////////////////////////////////////////////////////
//  HfsiInst.c
//  Implementation of the HFSI Data
//  Created on:      2015/11/09 15:41:54
//  Original author: Auto Generate By Excel
///////////////////////////////////////////////////////////

#include "HfsiInst.h"
#include "HfsiRam.h"
#include "EHfsiItemID.h"

static const CHfsiHfsiSpec hfsi_paper_rev_sub_paddle = { &(hfsiRamArea[0]), 957, 801, "Paper Rev Sub Paddle", 20, 900000, };
static const CHfsiHfsiSpec hfsi_paper_rev_paddle_lif = { &(hfsiRamArea[1]), 957, 802, "Paper Rev Paddle Lif", 20, 900000, };
static const CHfsiHfsiSpec hfsi_stacker_stapler_life = { &(hfsiRamArea[2]), 957, 804, "Stacker Stapler Life", 20, 500000, };

const CHfsiHFSI hfsiAllData[HFSIID_NUM] = {
    {&hfsi_paper_rev_sub_paddle},
    {&hfsi_paper_rev_paddle_lif},
    {&hfsi_stacker_stapler_life},
};

