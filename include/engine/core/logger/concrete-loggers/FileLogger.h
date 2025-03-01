/**************************************************************************************************
 * @file   FileLogger.h
 * @author Valentin Dumitru
 * @date   2024-04-17
 * @brief  @TODO Add description of this file if needed
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/

#pragma once
#include <fstream>
#include "engine/core/logger/ILogger.h"
#define LOG_FILE "log.txt"

class FileLogger : public ILogger
{
public:
    FileLogger();
    
    ~FileLogger() override;

    void log(const std::string &text) override;

    void error(const std::string &text) override;
    
    void warning(const std::string &text) override;

    void nonImportantInfo(const std::string& text) override;

    void info(const std::string &text) override;
    
    void infoBlue(const std::string &text) override;
    
    void infoPurple(const std::string &text) override;

    void importantInfo(const std::string &text) override;

    void importantInfoBlue(const std::string &text) override;
    
    void importantInfoPurple(const std::string &text) override;

    void importantInfoWhite(const std::string &text) override;
    
    void importantSuccess(const std::string &text) override;
    
    void success(const std::string &text) override;
private:
    void clearFileContent() ;
    std::ofstream logFile;
};