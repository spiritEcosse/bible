message(STATUS "========================Start configure ${LIB_AWS}====================")

set(AWS_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_AWS}")
set(AWS_INSTALL_DIR "${AWS_BUNDLE_DIR}/aws-cli")
set(AWS_BIN_DIR "${AWS_BUNDLE_DIR}/bin")

if(NOT EXISTS "${AWS_INSTALL_DIR}")
    execute_process(COMMAND bash -c "\
        mkdir -p ${AWS_BUNDLE_DIR}")

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "armv7l")
            set(AWS_ARCH "aarch64")
        elseif (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
            set(AWS_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
        endif()

        execute_process(COMMAND bash -c "\
            curl \"https://awscli.amazonaws.com/awscli-exe-linux-${AWS_ARCH}.zip\" -o \"awscliv2.zip\" && \
            unzip awscliv2.zip && \
            ./aws/install --install-dir ${AWS_INSTALL_DIR} --bin-dir ${AWS_BIN_DIR} --update"
            WORKING_DIRECTORY ${AWS_BUNDLE_DIR} RESULT_VARIABLE AWS_INSTALL_OUT)
        message("AWS_INSTALL_OUT ${AWS_INSTALL_OUT}")
        set(ENV{PATH} "${AWS_BIN_DIR}:$ENV{PATH}")
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        execute_process(COMMAND bash -c "\
            echo \"<?xml version=\"1.0\" encoding=\"UTF-8\"?>
            <!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">
            <plist version=\"1.0\">
              <array>
                <dict>
                  <key>choiceAttribute</key>
                  <string>customLocation</string>
                  <key>attributeSetting</key>
                  <string>${AWS_BUNDLE_DIR}</string>
                  <key>choiceIdentifier</key>
                  <string>default</string>
                </dict>
              </array>
            </plist>\" > choices.xml && \
            curl \"https://awscli.amazonaws.com/AWSCLIV2.pkg\" -o \"AWSCLIV2.pkg\" && \
            installer -pkg ./AWSCLIV2.pkg -target CurrentUserHomeDirectory -applyChoiceChangesXML choices.xml"
            WORKING_DIRECTORY ${AWS_BUNDLE_DIR} RESULT_VARIABLE AWS_INSTALL_OUT)
        message("AWS_INSTALL_OUT ${AWS_INSTALL_OUT}")
    endif()
endif()


set(ENV{PATH} "${AWS_INSTALL_DIR}:$ENV{PATH}")
execute_process(COMMAND bash -c "which aws" RESULT_VARIABLE AWS_OUT)
message("AWS_OUT ${AWS_OUT}")
execute_process(COMMAND bash -c "aws --version" RESULT_VARIABLE AWS_OUT)
message("AWS_OUT_VER ${AWS_OUT}")

message(STATUS "========================End configure ${LIB_AWS}====================")
