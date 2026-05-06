#include "task.h"

#include <stdint.h>
#include <stdio.h>

#include "log.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

int32_t find_unused_cell(TASK* ctrl) {
  if (ctrl == NULL) {
    return -1;  // Return -1 if ctrl is NULL
  } else if (ctrl->task_num >= MAX_TASK_NUM) {
    return -1;
  } else {
    for (int i = 0; i < MAX_TASK_NUM; i++) {
      if (ctrl->waiting[i] == false) {
        return i;
      }
    }
  }
}

bool add_task(TASK* ctrl, void (*task)(void* para), void* para, bool(*judge), void* cur_addr, float offset, TYPE type, uint32_t timeout) {
  int32_t pos = find_unused_cell(ctrl);
  if (pos == -1) {
    return false;
  } else {
    ctrl->cell[pos].task = task;
    ctrl->cell[pos].para = para;
    ctrl->cell[pos].judge = judge;
    ctrl->cell[pos].tar = get_cur_val(cur_addr, type) + offset;
    ctrl->cell[pos].cur_addr = cur_addr;
    ctrl->cell[pos].type = type;
    ctrl->cell[pos].timeout = ctrl->task_clock + timeout;
    ctrl->waiting[pos] = true;
    ctrl->task_num++;
    return true;
  }
}

bool is_timeout(TASK* ctrl, task_cell* cell) {
  if (cell->timeout <= 0) {
    return false;
  }
  if (ctrl->task_clock >= cell->timeout) {
    return true;
  }
  return false;
}

void driver_task(TASK* ctrl) {
  if (ctrl == NULL) {
    return;  // Return if ctrl is NULL
  } else if (ctrl->task_num == 0) {
    return;
  }
  for (int i = 0; i < MAX_TASK_NUM; i++) {
    if (ctrl->waiting[i] == true) {
      ctrl->cell[i].cur = get_cur_val(ctrl->cell[i].cur_addr, ctrl->cell[i].type);
      if (ctrl->cell[i].judge(ctrl->cell[i].cur, ctrl->cell[i].tar) || is_timeout(ctrl, &ctrl->cell[i])) {
        ctrl->cell[i].task(ctrl->cell[i].para);
        ctrl->waiting[i] = false;
        ctrl->task_num--;
      }
    }
  }
}

void init_TASK(TASK* ctrl) {
  if (ctrl == NULL) {
    return;  // Return if ctrl is NULL
  }
  ctrl->task_num = 0;
  ctrl->task_clock = 0;
  for (int i = 0; i < MAX_TASK_NUM; i++) {
    ctrl->waiting[i] = false;
    ctrl->cell[i].task = NULL;
    ctrl->cell[i].para = NULL;
    ctrl->cell[i].judge = NULL;
    ctrl->cell[i].cur_addr = NULL;
    ctrl->cell[i].type = UNKNOWN;
    ctrl->cell[i].timeout = 0;
  }
}

void update_task_clock(TASK* ctrl, uint64_t system_time) {
  if (ctrl == NULL) {
    return;  // Return if ctrl is NULL
  }
  ctrl->task_clock = system_time;
}

bool Greater_or_Equal(float cur, float tar) {
  if (cur >= tar) {
    return true;
  } else {
    return false;
  }
}

bool Less_or_Equal(float cur, float tar) {
  if (cur <= tar) {
    return true;
  } else {
    return false;
  }
}

bool Error_less_1(float cur, float tar) {
  if (ABS(cur - tar) < 1.0f) {
    return true;
  } else {
    return false;
  }
}

bool Error_less_10(float cur, float tar) {
  if (ABS(cur - tar) < 10.0f) {
    return true;
  } else {
    return false;
  }
}

bool Error_less_100(float cur, float tar) {
  if (ABS(cur - tar) < 100.0f) {
    return true;
  } else {
    return false;
  }
}

bool Error_less_1000(float cur, float tar) {
  if (ABS(cur - tar) < 1000.0f) {
    return true;
  } else {
    return false;
  }
}

bool All_false(float cur, float tar) {
  return false;
}

uint32_t add_task_with_time_line(TASK* ctrl, void (*task)(void* para), void* para, uint32_t later) {
  add_task(ctrl, task, para, All_false, NULL, 0, UINT8, later);
  return later;
}
