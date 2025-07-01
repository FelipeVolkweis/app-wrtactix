#include "runlambda.hh"

using namespace WRBeT;

RunLambda::RunLambda(QString name, std::function<void()> lambda) : ExecutionNode(name), lambda_(std::move(lambda)) {}

Status RunLambda::tick() {
    if (lambda_) {
        lambda_();
        return Status::SUCCESS;
    }
    return Status::FAILURE;
}