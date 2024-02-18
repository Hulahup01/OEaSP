#!/bin/bash

stty intr ^?

maze_file="maze.txt"
screamer_file="prize.txt"
sound_file="gg.wav"

player_char="🟦"
wall_char="⬛"
exit_char="🟩"
empty_char="⬜️"
key_char="🔑"
door_char="🔒"

rows=$(wc -l < "$maze_file")
cols=$(($(head -n 1 "$maze_file" | wc -c) - 1))

player_row=1
player_col=1

door_key=0

maze_text=$(<"$maze_file")

IFS=$'\n' read -r -d '' -a maze <<< "$maze_text"

maze[$player_row]=${maze[$player_row]:0:$player_col}@${maze[$player_row]:$((player_col + 1))}
    
display_maze() {
    graphic_maze=${1//"#"/"$wall_char"}
    graphic_maze=${graphic_maze//" "/"$empty_char"}   
    graphic_maze=${graphic_maze//"E"/"$exit_char"}    
    graphic_maze=${graphic_maze//"K"/"$key_char"}   
    graphic_maze=${graphic_maze//"D"/"$door_char"}   
    graphic_maze=${graphic_maze//"@"/"$player_char"}
    clear
    echo "$graphic_maze"
}


finish() {
    clear
    cat "$screamer_file"
    aplay "$sound_file"
    clear
    exit 0
}


handle_input() {
    read -rsn1 key
    case "$key" in
        "A")
            if [[ "${maze[$((player_row-1))]:$player_col:1}" != "#" ]]; then
                if [[ "${maze[$((player_row-1))]:$player_col:1}" == "D"  && $door_key -eq 0 ]]; then
                    return
                fi
                if [[ "${maze[$((player_row-1))]:$player_col:1}" == "K" ]]; then
                    door_key=1
                fi
                if [[ "${maze[$((player_row-1))]:$player_col:1}" == "E" ]]; then
                    finish
                fi
                maze[$player_row]=${maze[$player_row]:0:$player_col}" "${maze[$player_row]:$((player_col + 1))}
                ((player_row--))
                maze[$player_row]=${maze[$player_row]:0:$player_col}@${maze[$player_row]:$((player_col + 1))}
            fi
            ;;
        "B")
            if [[ "${maze[$((player_row+1))]:$player_col:1}" != "#" ]]; then
                if [[ "${maze[$((player_row+1))]:$player_col:1}" == "D" && $door_key -eq 0 ]]; then
                    return
                fi
                if [[ "${maze[$((player_row+1))]:$player_col:1}" == "K" ]]; then
                    door_key=1
                fi
                if [[ "${maze[$((player_row+1))]:$player_col:1}" == "E" ]]; then
                    finish
                fi
                maze[$player_row]=${maze[$player_row]:0:$player_col}" "${maze[$player_row]:$((player_col + 1))}
                ((player_row++))
                maze[$player_row]=${maze[$player_row]:0:$player_col}@${maze[$player_row]:$((player_col + 1))}
            fi
            ;;
        "C")
            if [[ "${maze[$player_row]:$((player_col+1)):1}" != "#" ]]; then
                if [[ "${maze[$player_row]:$((player_col+1)):1}" == "D" && $door_key -eq 0 ]]; then
                    return
                fi
                if [[ "${maze[$player_row]:$((player_col+1)):1}" == "K" ]]; then
                    door_key=1
                fi
                if [[ "${maze[$player_row]:$((player_col+1)):1}" == "E" ]]; then
                    finish
                fi
                maze[$player_row]=${maze[$player_row]:0:$player_col}" "${maze[$player_row]:$((player_col + 1))}
                ((player_col++))
                maze[$player_row]=${maze[$player_row]:0:$player_col}@${maze[$player_row]:$((player_col + 1))}
            fi
            ;;
        "D")
            if [[ "${maze[$player_row]:$((player_col-1)):1}" != "#" ]]; then
                if [[ "${maze[$player_row]:$((player_col-1)):1}" == "D" && $door_key -eq 0 ]]; then
                    return
                fi
                if [[ "${maze[$player_row]:$((player_col-1)):1}" == "K" ]]; then
                    door_key=1
                fi
                if [[ "${maze[$player_row]:$((player_col-1)):1}" == "E" ]]; then
                    finish
                fi
                maze[$player_row]=${maze[$player_row]:0:$player_col}" "${maze[$player_row]:$((player_col + 1))}
                ((player_col--))
                maze[$player_row]=${maze[$player_row]:0:$player_col}@${maze[$player_row]:$((player_col + 1))}
            fi
            ;;
        *)
            ;;
    esac

    maze_str=$(IFS=$'\n'; echo "${maze[*]}")

    display_maze "$maze_str"
}

maze_str=$(IFS=$'\n'; echo "${maze[*]}")
display_maze "$maze_str"

while true; do
    handle_input
done
