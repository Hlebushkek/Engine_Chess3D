CREATE TABLE IF NOT EXISTS `chess3d`.`lobby` (
  `id` INT NOT NULL,
  `user_white_id` INT NOT NULL,
  `user_black_id` INT NOT NULL,
  `name` VARCHAR(32) NOT NULL,
  `password` VARCHAR(32) NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE,
  INDEX `user_white_id_idx` (`user_white_id` ASC) VISIBLE,
  INDEX `user_black_id_idx` (`user_black_id` ASC) VISIBLE,
  CONSTRAINT `user_white_id`
    FOREIGN KEY (`user_white_id`)
    REFERENCES `chess3d`.`user` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `user_black_id`
    FOREIGN KEY (`user_black_id`)
    REFERENCES `chess3d`.`user` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);