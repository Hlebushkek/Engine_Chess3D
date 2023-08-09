CREATE TABLE IF NOT EXISTS `chess3d`.`spectator` (
  `id` INT NOT NULL,
  `user_id` INT NOT NULL,
  `lobby_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `idspectator_UNIQUE` (`id` ASC) VISIBLE,
  INDEX `lobby_id_idx` (`lobby_id` ASC) VISIBLE,
  CONSTRAINT `user_id`
    FOREIGN KEY (`user_id`)
    REFERENCES `chess3d`.`user` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `lobby_id`
    FOREIGN KEY (`lobby_id`)
    REFERENCES `chess3d`.`lobby` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);
