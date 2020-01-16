REPO=shevchenkoigor/bible
TAG?=latest
include .env
export $(shell sed 's/=.*//' .env)

coverage:
	docker-compose up
	xdg-open tests/${HTML}/index.html

tests:
	cd tests && \
    qmake tests.pro && \
    make && \
    xvfb-run -a ./tests && \
	
build:
	docker build -t ${REPO}:${TAG} .

push: build
	docker push ${REPO}:${TAG}

up_hard:
	docker-compose stop \
	&& docker-compose rm -f \
	&& docker image rm -f ${REPO}:${TAG} \
	&& docker build -t ${REPO}:${TAG} . \
	&& docker-compose up

build_push:
	docker build -t ${REPO}:${TAG} .
	docker push ${REPO}:${TAG}

commit:
	git add . && \
	git commit -m '${COMMIT_MESSAGE}' && \
	git push  && \
	git branch --set-upstream-to=origin/`git rev-parse --abbrev-ref HEAD` `git rev-parse --abbrev-ref HEAD` && \
	docker build -t ${REPO}:${TAG} . && \
	docker push ${REPO}:${TAG}
