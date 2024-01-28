INCLUDE(FetchContent)

SET(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)

SET(SFML_VERSION "2.6.x")
SET(EnTT_VERSION "v3.12.x")
SET(json_VERSION "master")
SET(glm_VERSION "master")
SET(b2d_VERSION "main")
SET(efsw_VERSION "master")

# SFML
FETCHCONTENT_DECLARE(
  SFML
  GIT_REPOSITORY        https://github.com/SFML/SFML.git
  GIT_TAG               ${SFML_VERSION}
  SYSTEM
)
# json
FETCHCONTENT_DECLARE(
  nlohmannJson
  GIT_REPOSITORY        https://github.com/nlohmann/json.git
  GIT_TAG               ${json_VERSION}
  SYSTEM
)
# glm
FETCHCONTENT_DECLARE(
  glm
  GIT_REPOSITORY        https://github.com/g-truc/glm.git
  GIT_TAG               ${glm_VERSION}
  SYSTEM
)
# EnTT
FETCHCONTENT_DECLARE(
  EnTT
  GIT_REPOSITORY        https://github.com/skypjack/entt.git
  GIT_TAG               ${EnTT_VERSION}
  SYSTEM
)

# Box2D
FETCHCONTENT_DECLARE(
  b2d
  GIT_REPOSITORY        https://github.com/erincatto/box2d.git
  GIT_TAG               ${b2d_VERSION}
  SYSTEM
)
# efsw
FETCHCONTENT_DECLARE(
  efsw
  GIT_REPOSITORY        https://github.com/SpartanJ/efsw.git
  GIT_TAG               ${efsw_VERSION}
  SYSTEM
)

# Build
FETCHCONTENT_MAKEAVAILABLE(SFML nlohmannJson glm EnTT b2d efsw)