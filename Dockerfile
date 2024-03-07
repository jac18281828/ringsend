FROM jac18281828/cpp:latest

ARG PROJECT=ringsend
WORKDIR /workspaces/${PROJECT}


COPY --chown=cpp:cpp . .
USER cpp

ENV PATH=${PATH}:/usr/local/go/bin
RUN yamlfmt -lint .github/workflows/*.yml

ARG BUILD=build
ARG TYPE=DEBUG
RUN mkdir -p ${BUILD}
#build
ENV MAKEFLAGS=-j8
ENV PROJECT_NAME=${PROJECT}
RUN cmake -H. -B${BUILD} -DPROJECT_NAME=${PROJECT} -DCMAKE_BUILD_TYPE=${TYPE} -DCMAKE_VERBOSE_MAKEFILE=on -Wno-dev "-GUnix Makefiles"
RUN cmake --build ${BUILD} --verbose --config ${TYPE}
ENV CTEST_OUTPUT_ON_FAILURE=1
RUN (cd ${BUILD} && ctest)

CMD build/ringsend 3 3000
