INCLUDE(FetchContent)

SET(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS)

SET(SFML_VERSION "2.6.x")
SET(LDtkLoader_VERSION "1.2")
SET(EnTT_VERSION "v3.12.x")

SET(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS)

# SFML
FETCHCONTENT_DECLARE(
  SFML
  GIT_REPOSITORY        https://github.com/SFML/SFML.git
  GIT_TAG               ${SFML_VERSION}
  FIND_PACKAGE_ARGS     NAMES SFML ${SFML_VERSION} COMPONENTS graphics audio
)
# LDtkLoader
FETCHCONTENT_DECLARE(
  LDtkLoader
  GIT_REPOSITORY        https://github.com/Madour/LDtkLoader.git
  GIT_TAG               07d0ce00a017b0cec4548aa20a63b4b26ceac009
  FIND_PACKAGE_ARGS     NAMES LDtkLoader ${LDtkLoader_VERSION}
)
# jsoncpp
FETCHCONTENT_DECLARE(
  jsoncpp
  GIT_REPOSITORY        https://github.com/open-source-parsers/jsoncpp.git
  GIT_TAG               69098a18b9af0c47549d9a271c054d13ca92b006
)
# glm
FETCHCONTENT_DECLARE(
  glm
  GIT_REPOSITORY        https://github.com/g-truc/glm.git
  GIT_TAG               47585fde0c49fa77a2bf2fb1d2ead06999fd4b6e
)
# EnTT
FETCHCONTENT_DECLARE(
  EnTT
  GIT_REPOSITORY        https://github.com/skypjack/entt.git
  GIT_TAG               ${EnTT_VERSION}
)

# Build
FETCHCONTENT_MAKEAVAILABLE(SFML LDtkLoader jsoncpp glm EnTT)