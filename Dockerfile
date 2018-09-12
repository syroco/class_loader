FROM alpine
RUN apk add --no-cache cmake g++ make
RUN mkdir -p class_loader/build
COPY ./ class_loader/
WORKDIR class_loader/build
RUN rm -rf *
RUN cmake ..
RUN cmake --build . -- -j`nproc`
RUN cmake --build . --target install
