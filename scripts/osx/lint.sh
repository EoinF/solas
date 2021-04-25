set -ex

FILTERS="-whitespace/tab,-whitespace/braces,-whitespace/indent,-whitespace/comments,"
FILTERS+="-legal/copyright,-runtime/reference,-readability/todo"
PATHS="../SolasLib"

pushd scripts
    pipenv install
    pipenv run cpplint --recursive --linelength=100 --filter $FILTERS $PATHS
popd