#pragma once

#include "head.h"
#include <ESP_Mail_Client.h>
#define SMTP_HOST "smtp.office365.com"

#define SMTP_PORT esp_mail_smtp_port_587 // port 465 is not available for Outlook.com

#define AUTHOR_EMAIL "Bsgbsg7@Outlook.com"
#define AUTHOR_PASSWORD "suyqnddqnzoxtlql"
/* Recipient email address */
#define RECIPIENT_EMAIL "2028163967@qq.com"
lv_obj_t *fire_load();
void smtpCallback(SMTP_Status status);
void smtp_init();
void smtpCallback(SMTP_Status status);
void fire_gesture(lv_event_t *e);
void fire_timer_cb(lv_timer_t *timer);









