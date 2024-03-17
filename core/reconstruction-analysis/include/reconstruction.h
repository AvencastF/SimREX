//
// Created by Yulei on 2024/3/14.
//

#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <string>

namespace SimREX::Reconstruction {
    void run_reconstruction(
        const std::string& _config_path, int _random_seed, int _threads, const std::string& _log_file
    );
}


#endif //RECONSTRUCTION_H
