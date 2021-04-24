set -ex

FILTERS="-whitespace/tab,-whitespace/braces,-legal/copyright,-whitespace/indent"
PATHS="../SolasLib ../ScenarioViewer"

pushd scripts
    pipenv install
    pipenv run cpplint --recursive --linelength=100 --filter $FILTERS $PATHS
popd