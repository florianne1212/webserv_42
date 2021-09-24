# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/16 08:17:15 by ldutriez          #+#    #+#              #
#    Updated: 2021/09/20 11:15:57 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

CC =		clang++

SRC_DIR = 	$(shell find srcs -type d)
INC_DIR = 	$(shell find srcs -type d)
OBJ_DIR = obj

vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

#SRC = $(foreach dir, $(SRC_DIR), $(foreach file, $(wildcard $(dir)/*.cpp), $(notdir $(file))))
SRC = main.cpp Config.cpp Server.cpp Routes.cpp ASocket.cpp ClientSocket.cpp FDList.cpp ListeningSocket.cpp CgiSocket.cpp CgiSocketFromCgi.cpp CgiSocketToCgi.cpp ParseHeaderFields.cpp \
ParseRequest.cpp ParseBody.cpp ParseChunkedBody.cpp Request.cpp Response.cpp DeleteMethod.cpp Mime.cpp Listmime.cpp \
HttpStatus.cpp GetMethod.cpp File.cpp IMiddleware.cpp MiddlewareChain.cpp MethodMiddleware.cpp  CheckRedir.cpp \
IsConnected.cpp CheckRequest.cpp ManageMiddleware.cpp PostMethod.cpp CgiHandler.cpp Buffer.cpp CheckRoot.cpp CheckBodySize.cpp CheckPath.cpp GetFileError.cpp

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 #-fsanitize=address

IFLAGS = $(foreach dir, $(INC_DIR), -I$(dir))
# Colors

_GREY=	$'\033[30m
_RED=	$'\033[31m
_GREEN=	$'\033[32m
_YELLOW=$'\033[33m
_BLUE=	$'\033[34m
_PURPLE=$'\033[35m
_CYAN=	$'\033[36m
_WHITE=	$'\033[37m

all:			$(NAME)

show:
	@echo "$(_BLUE)SRC :\n$(_YELLOW)$(SRC)$(_WHITE)"
	@echo "$(_BLUE)OBJ :\n$(_YELLOW)$(OBJ)$(_WHITE)"
	@echo "$(_BLUE)CFLAGS :\n$(_YELLOW)$(CFLAGS)$(_WHITE)"
	@echo "$(_BLUE)IFLAGS :\n$(_YELLOW)$(IFLAGS)$(_WHITE)"
	@echo "$(_BLUE)INC_DIR :\n$(_YELLOW)$(INC_DIR)$(_WHITE)"

$(OBJ_DIR)/%.o : %.cpp
				@echo "Compiling $(_YELLOW)$@$(_WHITE) ... \c"
				@mkdir -p $(OBJ_DIR)
				@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<
				@echo "$(_GREEN)DONE$(_WHITE)"

$(NAME): 		$(INC_DIR) $(OBJ) Makefile
				@echo "-----\nCreating Executable $(_YELLOW)$@$(_WHITE) ... \c"
				@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
				@echo "$(_GREEN)DONE$(_WHITE)\n-----"

re:				fclean all

clean:
				@echo "$(_WHITE)Deleting Object $(_YELLOW)$(OBJ_DIR)$(_WHITE) ... \c"
				@rm -rf $(OBJ_DIR)
				@echo "$(_GREEN)DONE$(_WHITE)\n-----"

fclean:			clean
				@echo "Deleting Binary File $(_YELLOW)$(NAME)$(_WHITE) ... \c"
				@rm -f $(NAME)
				@echo "$(_GREEN)DONE$(_WHITE)\n-----"

docker-build:	all
				docker build -t webserv-run -f docker/Dockerfile .

docker-run: 	docker-build
				docker run --rm -it --network host webserv-run

.PHONY: all clean flcean re show docker-build docker-run
