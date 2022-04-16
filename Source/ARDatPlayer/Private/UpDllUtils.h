// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#define WIN32_LEAN_AND_MEAN





int dlCmd(int cmd, int pm1, int pm2);
int dlSendCmd(int cmd, int64_t pm1, int64_t pm2, const  wchar_t* pwc = nullptr, bool sync = true);