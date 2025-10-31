#include "retro_fortress/server/logging.hpp"

#include "retro_fortress/common/goldsrc_stub.hpp"

namespace rf2::server {

void log_info(const std::string& message) {
    rf2::goldsrc::ServerPrint((message + "\n").c_str());
}

}  // namespace rf2::server

